/****************************************Copyright (c)****************************************************
**                                        
**                                 
**
**--------------File Info---------------------------------------------------------------------------------
** File name:			
** Last modified Date:          
** Last Version:		   
** Descriptions:							
**--------------------------------------------------------------------------------------------------------
** Created by:			FiYu
** Created date:		2015-12-11
** Version:			    1.0
** Descriptions:		nRF24L01无线发送数据实验				
**--------------------------------------------------------------------------------------------------------
** Modified by:			
** Modified date:		
** Version:				
** Descriptions:		
** Rechecked by:			
**********************************************************************************************************/
/****-----请阅读ReadMe.txt进行实验-----***********/

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "nRF24l01P.h"
#include "led.h"
#include "CRC.h"
#include "timer.h"

const uint16_t M100_ID = 0x0003;
static uint8_t low_voltage_count = 0;
u8  downside_data_length = 12;
u8  TxPayload[19] = "888888888888";   //无线发送缓存
u8 	downside_data[19] = "111111111111";
volatile uint8_t downside_order_Led = 0;
volatile uint8_t Ele_Data_Send = 0;

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Enable the GPIO_LED Clock */
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE); 

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
//	GPIO_DeInit(GPIOB);	 //将外设GPIOB寄存器重设为缺省值
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_DeInit(GPIOB);	 //将外设GPIOB寄存器重设为缺省值
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;   //推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_DeInit(GPIOA);	 //将外设GPIOB寄存器重设为缺省值
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_6 |GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;    //浮空输入   
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA , GPIO_Pin_11);   //485初始状态，RE为低为输出状态。反相与否？
	GPIO_SetBits(GPIOA , GPIO_Pin_15);   //初始状态，熄灭指示灯

}
/***************************************************************************
 * 描  述 : MAIN函数
 * 入  参 : 无
 * 返回值 : 无
 **************************************************************************/
int main(void)
{
	uint8_t sys_led_flag = 0;
	u16 CRC_tmp_code = 0;
	uint8_t BBB_number = 0;
	uint8_t upside_data_tmp[19] = "111111111111111111";
	uint16_t ADC8_data = 1;
	uint8_t ADC_code[5] = {0xff,0xff};
	uint8_t ADC_String[7] = {0};
	uint8_t i = 0;
	uint32_t sum = 0;
	float y = 0;
	
	GPIO_Configuration();//Init_NRF24L01_MB();	        //初始化
	
	USART_Configuration();
	USART1_Init();
	ADC_Init_E();
	TIM3_Init();                 //初始化配置TIM
	GPIO_SetBits(GPIOA , GPIO_Pin_11);
	
	LED_Toggle(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	delay_ms(500);
	LED_Toggle(GPIOA,GPIO_Pin_2);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	delay_ms(500);
	LED_Toggle(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	delay_ms(500);
	LED_Toggle(GPIOA,GPIO_Pin_2);
	GPIO_SetBits(GPIOA,GPIO_Pin_4);
	GPIO_SetBits(GPIOA,GPIO_Pin_5);
	GPIO_SetBits(GPIOA,GPIO_Pin_6);
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	GPIO_SetBits(GPIOA,GPIO_Pin_8);
	delay_ms(500);
	LED_Toggle(GPIOA,GPIO_Pin_2);
	GPIO_ResetBits(GPIOA,GPIO_Pin_4);
	GPIO_ResetBits(GPIOA,GPIO_Pin_5);
	GPIO_ResetBits(GPIOA,GPIO_Pin_6);
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	delay_ms(500);
	GPIO_ResetBits(GPIOA, GPIO_Pin_2);
  while(1)
  {	
	  for(i = 0; i< 100; i++)
	  { 
		sum += Get_AdcData(8);
		delay_ms(1);
	  }
	  ADC8_data = sum/100;
	  sum = 0;
	 
//	  ADC8_data =  Get_AdcData(8);
	  ADC_code[1] = (uint8_t)(ADC8_data >> 8);
	  ADC_code[0] = (uint8_t)ADC8_data;
		if(Ele_Data_Send == 1)
		{
			sprintf(ADC_code,"%04x\n",ADC8_data);
			UsartSendDat(ADC_code,sizeof(ADC_code));
			Ele_Data_Send = 0;
		}			
//	  delay_ms(10);
//	  ADC8_data = 2650;
	  y = 144.0/24800*ADC8_data -(144.0/24800)*2650+12.6;
	
	  sprintf(ADC_String,"%.2fV\n",y);
		UsartSendDat(ADC_String,sizeof(ADC_String)); 
		if(y >= 11.79)
		{
			GPIO_ResetBits(GPIOA , GPIO_Pin_4);     //LED1
			GPIO_ResetBits(GPIOA , GPIO_Pin_5);			//LED2
			GPIO_ResetBits(GPIOA , GPIO_Pin_6);			//LED3
			GPIO_ResetBits(GPIOA , GPIO_Pin_7);			//LED4
		}else if(y < 11.79 && y >= 11.37)
		{
			GPIO_SetBits(GPIOA , GPIO_Pin_4);
			GPIO_ResetBits(GPIOA , GPIO_Pin_5);
			GPIO_ResetBits(GPIOA , GPIO_Pin_6);
			GPIO_ResetBits(GPIOA , GPIO_Pin_7);
		}else if(10.2 <= y && y < 11.37)
		{
			GPIO_SetBits(GPIOA , GPIO_Pin_4);
			GPIO_SetBits(GPIOA , GPIO_Pin_5);
			GPIO_ResetBits(GPIOA , GPIO_Pin_6);
			GPIO_ResetBits(GPIOA , GPIO_Pin_7);
		}else 
		if(y < 10.2)  //10.5v y < 10.5
		{
			GPIO_SetBits(GPIOA , GPIO_Pin_4);
			GPIO_SetBits(GPIOA , GPIO_Pin_5);
			GPIO_SetBits(GPIOA , GPIO_Pin_6);
//			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
			if(BBB_number < 6 || (BBB_number >= 32 && BBB_number <= 33)) //凑 两次报警的10s延时间隔  凤鸣间隔加长 30s重复 嘟嘟三声
			{
				 if(BBB_number < 2 || BBB_number >= 32)
				 LED_Toggle(GPIOA,GPIO_Pin_2);
				 else GPIO_ResetBits(GPIOA, GPIO_Pin_2);
//				GPIO_ResetBits(GPIOA, GPIO_Pin_2);
//				delay_ms(500);
//				GPIO_SetBits(GPIOA, GPIO_Pin_2);
//				delay_ms(500);
//				GPIO_ResetBits(GPIOA, GPIO_Pin_2);
//				delay_ms(500);
//				GPIO_SetBits(GPIOA, GPIO_Pin_2);
//				delay_ms(500);
//				GPIO_ResetBits(GPIOA, GPIO_Pin_2);
//				delay_ms(500);
//				GPIO_SetBits(GPIOA, GPIO_Pin_2);
//				delay_ms(500);			
			}else GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		}
		if(y >= 10.2)	//y >= 10.5
		{
			delay_ms(900);
			LED_Toggle(GPIOA,GPIO_Pin_15);
			LED_Toggle(GPIOA,GPIO_Pin_8);
			
			GPIO_ResetBits(GPIOA, GPIO_Pin_2);
			if(BBB_number >= 33)
			BBB_number = 0;
		}else 
		{
			sys_led_flag ++;
			delay_ms(200);
			if(sys_led_flag%2 == 0)
			{
				LED_Toggle(GPIOA,GPIO_Pin_7);	
			}
			if((BBB_number == 1 || BBB_number == 33) && sys_led_flag == 2)
			{
				sys_led_flag = 4;	
			}
			if(sys_led_flag == 4)
			{
				BBB_number ++;
				if(BBB_number == 34)
				{
					BBB_number = 2;
				}
				LED_Toggle(GPIOA,GPIO_Pin_15);
				LED_Toggle(GPIOA,GPIO_Pin_8);			
				sys_led_flag = 0; 
			}
		}
		
	}
}
/*********************************END FILE************************************/