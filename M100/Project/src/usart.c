#include "stm32f10x.h"
#include "usart.h"

volatile unsigned char UsartReceFlag = 0;
unsigned char UsartReceBuf[USART_RECE_BUF_SIZE];//Usart接收缓存
volatile unsigned int UsartReceCnt = 0;              //Usart接收计数

extern volatile uint8_t downside_order_Led;
extern volatile uint8_t Ele_Data_Send;

uint8_t upside_data[19] = "111111111111111111";
void USART1_Init(void)
{
	USART_InitTypeDef				USART_InitStructure;
	USART_ClockInitTypeDef  USART_ClockInitStructure;
	NVIC_InitTypeDef				NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE);  
	
	USART_DeInit(USART1);          //将外设USART1寄存器重设为缺省值
	USART_InitStructure.USART_BaudRate = 9600;     //设置串口1波特率为9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //设置一个帧中传输数据位 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //定义发送的停止位数目为1
	USART_InitStructure.USART_Parity = USART_Parity_No;     //奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //发送接收使能
	USART_Init(USART1, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     //时钟低电平活动
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;         //设置SCLK引脚上时钟输出的极性为低电平
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;   //时钟第一个边沿进行数据捕获
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //最后一位数据的时钟脉冲不从SCLK输出
  USART_ClockInit(USART1, &USART_ClockInitStructure); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //使能USART1接收中断
	USART_Cmd(USART1, ENABLE);      //使能USART1外设
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //NVIC_Group:NVIC分组 0~4 总共5组,最高2位抢占
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************
 * 描  述 : USART1配置
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void USART_Configuration(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 

 /********************以下为USART1配置**************************/
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO,ENABLE);
  /*
  *  USART1_TX -> PA9 , USART1_RX ->	PA10
  */				
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	         
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);		   

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据
  USART_InitStructure.USART_StopBits = USART_StopBits_1;     //1位停止位
  USART_InitStructure.USART_Parity = USART_Parity_No;        //无校验位
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //禁止硬件流控制
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //发送和接收使能
  USART_Init(USART1, &USART_InitStructure); 

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  /***********************************************************************************
	  void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
	  使能或者失能USART外设
	  USARTx：x可以是1，2或者3，来选择USART外设
	  NewState: 外设USARTx的新状态
	  这个参数可以取：ENABLE或者DISABLE
  ***********************************************************************************/
  USART_Cmd(USART1, ENABLE); 
  USART_ClearITPendingBit(USART1, USART_IT_TC);//清除中断TC位
}
void USART1_IRQHandler(void)  
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//接收中断
	{	
		UsartReceBuf[UsartReceCnt++] = USART1->DR;//
		
		if(UsartReceBuf[0] != 0xA5)
		{
			UsartReceCnt = 0;
//		if(UsartReceBuf[10] == 'z')UsartReceFlag = 1;
//		if(UsartReceBuf[0] == 0x01 || UsartReceBuf[0] == 0x02 || UsartReceBuf[0] == 0x03)downside_order = UsartReceBuf[0];
			switch(UsartReceBuf[0])
			{
				case 0x01:
					Ele_Data_Send = 1;
					break;
				case 0x02:
					downside_order_Led = 1;
					break;
				case 0x03:
					downside_order_Led = 0;
					break;
			}
		}
		if(UsartReceCnt == 19)
		{
			memcpy(upside_data,UsartReceBuf,19);
			UsartReceCnt = 0;
			memset(UsartReceBuf,0,19);
			UsartReceFlag = 1;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	if (USART_GetITStatus(USART1, USART_IT_TXE) != RESET) 
	{
        USART_ClearITPendingBit(USART1, USART_IT_TXE);           /* Clear the USART transmit interrupt                  */
  }	
}
void UsartSendDat(unsigned char *send_buff,unsigned int length)
{
 	unsigned int i = 0;
	for(i = 0;i < length;i ++)
	{			
		USART1->DR = send_buff[i];
		while((USART1->SR&0X40)==0);	
	}	
}

/*ADC1初始化函数*/
void ADC_Init_E(void)
{
	RCC->APB2ENR |=1<<9;   //ADC1时钟使能
	RCC->APB2RSTR |=1<<9;  //ADC1复位
	RCC->APB2RSTR &=~(1<<9);  //复位结束
	RCC->CFGR &=~(3<<14);        //分频因子清零
	//SYSCLK/DIV2=12M ADC时钟设置为12M,ADC最大时钟不能超过14M!
	//否则将导致ADC准确度下降! 
	RCC->CFGR |=2<<14;       //
	ADC1->CR1 &=0XF0FFFF;    //工作模式清零
	ADC1->CR1 |=0<<16;       //独立工作模式
	ADC1->CR1 &=~(1<<8);      //非扫描模式
	ADC1->CR2 &=~(1<<1);      //单次转换模式         ADC1->CR2 |=1<<1; 连续扫描模式
	ADC1->CR2 &=~(7<<17);     //
	ADC1->CR2 |=7<<17;        //软件控制转换
	ADC1->CR2 |=1<<20;         //使用用外部触发(SWSTART)!!!	必须使用一个事件来触发
	ADC1->CR2 &=~(1<<11);      //右对齐
	ADC1->SQR1 &=~(0XF<<20);   //
	ADC1->SQR1 |=0<<20;        //1个转换在规则序列中，也就是只转换规则序列1  
	//设置通道1的采样时间
	ADC1->SMPR1 &=~(3*5);      //通道15采样时间清空
	ADC1->SMPR1 |=7<<(3*5);    //通道15   239.5周期，提高采样时间可以提高精确度
	ADC1->CR2 |=1<<0;           //开启AD转换器
	ADC1->CR2 |=1<<3;           //使能复位校准
	while(ADC1->CR2 &1<<3);     //等待校准结束 
	//该位由软件设置并由硬件清除，在校准寄存器被 初始化后该位将 被清除
	ADC1->CR2 |=1<<2;           //开启AD校准
	while(ADC1->CR2 &1<<2);     //等待校准结束 
	//该位由软件设置以开始校准，并在校准结束时由硬件清除
}
/*获得ADC1某个通道的值*/
uint16_t Get_AdcData(uint8_t ch)
{
	ADC1->SQR3 &=0XFFFFFFE0;         //
	ADC1->SQR3 |=ch;                 //
	ADC1->CR2 |=1<<22;               //
	while(!(ADC1->SR &1<<1));        //转换是否结束
	return ADC1->DR;
}

