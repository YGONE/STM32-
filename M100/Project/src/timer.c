
#include "timer.h"
#include "stm32f10x.h"

volatile u8 LED_TAG=0;   //定义LED切换标签，用于控制PB0
extern void LED_Toggle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
extern volatile uint8_t Ele_Data_Send;

/**************************************************************************************
 * 描  述 : 初始化TIM3并配置TIM3中断优先级
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void TIM3_Init(void)
{
	TIM_TimeBaseInitTypeDef			TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the PWR/BKP Clock */
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3 , ENABLE);  
	
	TIM_DeInit(TIM3);        //将外设TIM3寄存器重设为缺省值  
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1 ;    //设置了时钟分割(Tck_tim) 
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up ;   //选择了计数器模式(TIM向上计数模式)  
	TIM_TimeBaseInitStruct.TIM_Period = 9999 ;       //设定计数器自动重装值,取值范围0x0000~0xFFFF   
	TIM_TimeBaseInitStruct.TIM_Prescaler = 3599 ;    //设置用来作为TIM3时钟频率除数的预分频值为(7199+1),取值范围0x0000~0xFFFF 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct ) ;       

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);         //清除TIM3的待处理标志位    
	TIM_ITConfig(TIM3, TIM_IT_Update,ENABLE);     //使能TIM3中断  
	TIM_Cmd(TIM3, ENABLE);         //使能TIM3外设  
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);    //NVIC_Group:先占优先级2位，从优先级2位  
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;    //配置为TIM3中断  
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级为1  
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;   //从优先级为2  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;      //使能中断通道  
  NVIC_Init(&NVIC_InitStructure); 
}

/**************************************************************************************
 * 描  述 : LED2交替闪烁
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void LED_Blink(void)
{
	if(LED_TAG)
	{
	  GPIO_ResetBits(GPIOB , GPIO_Pin_9);  //LED2点亮
		LED_TAG=0;
	}
	else
	{
	  GPIO_SetBits(GPIOB , GPIO_Pin_9);    //LED2熄灭
		LED_TAG=1;
	}
}

/**************************************************************************************
 * 描  述 : TIM3全局中断服务
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************************/
void TIM3_IRQHandler(void)
{
	if(Ele_Data_Send == 1)
	{
		LED_Toggle(GPIOA,GPIO_Pin_8);
	}else if(Ele_Data_Send == 0)GPIO_SetBits(GPIOA, GPIO_Pin_8);    //LED2熄灭
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);   //清除中断标志位 
}

