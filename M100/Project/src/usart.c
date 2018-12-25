#include "stm32f10x.h"
#include "usart.h"

volatile unsigned char UsartReceFlag = 0;
unsigned char UsartReceBuf[USART_RECE_BUF_SIZE];//Usart���ջ���
volatile unsigned int UsartReceCnt = 0;              //Usart���ռ���

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
	
	USART_DeInit(USART1);          //������USART1�Ĵ�������Ϊȱʡֵ
	USART_InitStructure.USART_BaudRate = 9600;     //���ô���1������Ϊ9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //����һ��֡�д�������λ 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;     //���巢�͵�ֹͣλ��ĿΪ1
	USART_InitStructure.USART_Parity = USART_Parity_No;     //��żʧ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;  //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;   //���ͽ���ʹ��
	USART_Init(USART1, &USART_InitStructure);    

  USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;     //ʱ�ӵ͵�ƽ�
  USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;         //����SCLK������ʱ������ļ���Ϊ�͵�ƽ
  USART_ClockInitStructure.USART_CPHA = USART_CPHA_1Edge;   //ʱ�ӵ�һ�����ؽ������ݲ���
  USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;  //���һλ���ݵ�ʱ�����岻��SCLK���
  USART_ClockInit(USART1, &USART_ClockInitStructure); 
	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  //ʹ��USART1�����ж�
	USART_Cmd(USART1, ENABLE);      //ʹ��USART1����
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //NVIC_Group:NVIC���� 0~4 �ܹ�5��,���2λ��ռ
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
/**************************************************************************************
 * ��  �� : USART1����
 * ��  �� : ��
 * ����ֵ : ��
 **************************************************************************************/
void USART_Configuration(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
  USART_InitTypeDef USART_InitStructure; 

 /********************����ΪUSART1����**************************/
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
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8λ����
  USART_InitStructure.USART_StopBits = USART_StopBits_1;     //1λֹͣλ
  USART_InitStructure.USART_Parity = USART_Parity_No;        //��У��λ
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��ֹӲ��������
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //���ͺͽ���ʹ��
  USART_Init(USART1, &USART_InitStructure); 

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
  /***********************************************************************************
	  void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
	  ʹ�ܻ���ʧ��USART����
	  USARTx��x������1��2����3����ѡ��USART����
	  NewState: ����USARTx����״̬
	  �����������ȡ��ENABLE����DISABLE
  ***********************************************************************************/
  USART_Cmd(USART1, ENABLE); 
  USART_ClearITPendingBit(USART1, USART_IT_TC);//����ж�TCλ
}
void USART1_IRQHandler(void)  
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//�����ж�
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

/*ADC1��ʼ������*/
void ADC_Init_E(void)
{
	RCC->APB2ENR |=1<<9;   //ADC1ʱ��ʹ��
	RCC->APB2RSTR |=1<<9;  //ADC1��λ
	RCC->APB2RSTR &=~(1<<9);  //��λ����
	RCC->CFGR &=~(3<<14);        //��Ƶ��������
	//SYSCLK/DIV2=12M ADCʱ������Ϊ12M,ADC���ʱ�Ӳ��ܳ���14M!
	//���򽫵���ADC׼ȷ���½�! 
	RCC->CFGR |=2<<14;       //
	ADC1->CR1 &=0XF0FFFF;    //����ģʽ����
	ADC1->CR1 |=0<<16;       //��������ģʽ
	ADC1->CR1 &=~(1<<8);      //��ɨ��ģʽ
	ADC1->CR2 &=~(1<<1);      //����ת��ģʽ         ADC1->CR2 |=1<<1; ����ɨ��ģʽ
	ADC1->CR2 &=~(7<<17);     //
	ADC1->CR2 |=7<<17;        //�������ת��
	ADC1->CR2 |=1<<20;         //ʹ�����ⲿ����(SWSTART)!!!	����ʹ��һ���¼�������
	ADC1->CR2 &=~(1<<11);      //�Ҷ���
	ADC1->SQR1 &=~(0XF<<20);   //
	ADC1->SQR1 |=0<<20;        //1��ת���ڹ��������У�Ҳ����ֻת����������1  
	//����ͨ��1�Ĳ���ʱ��
	ADC1->SMPR1 &=~(3*5);      //ͨ��15����ʱ�����
	ADC1->SMPR1 |=7<<(3*5);    //ͨ��15   239.5���ڣ���߲���ʱ�������߾�ȷ��
	ADC1->CR2 |=1<<0;           //����ADת����
	ADC1->CR2 |=1<<3;           //ʹ�ܸ�λУ׼
	while(ADC1->CR2 &1<<3);     //�ȴ�У׼���� 
	//��λ��������ò���Ӳ���������У׼�Ĵ����� ��ʼ�����λ�� �����
	ADC1->CR2 |=1<<2;           //����ADУ׼
	while(ADC1->CR2 &1<<2);     //�ȴ�У׼���� 
	//��λ����������Կ�ʼУ׼������У׼����ʱ��Ӳ�����
}
/*���ADC1ĳ��ͨ����ֵ*/
uint16_t Get_AdcData(uint8_t ch)
{
	ADC1->SQR3 &=0XFFFFFFE0;         //
	ADC1->SQR3 |=ch;                 //
	ADC1->CR2 |=1<<22;               //
	while(!(ADC1->SR &1<<1));        //ת���Ƿ����
	return ADC1->DR;
}

