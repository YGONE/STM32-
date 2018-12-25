#include "stm32f10x.h"

#define	FALSE					0
#define	TRUE					1

#define  USART_RECE_BUF_SIZE  128


extern volatile unsigned char UsartReceFlag;
extern unsigned char UsartReceBuf[USART_RECE_BUF_SIZE];//Usart接收缓存
extern volatile unsigned int UsartReceCnt;              //Usart接收计数
extern void USART_Configuration(void);
extern void UsartSendDat(unsigned char *send_buff,unsigned int length);
extern uint8_t upside_data[19];
extern  void USART1_Init(void);
extern  void USART1_Tx_Puts(void);
extern void USART_Send_data(u8 *Str,unsigned int Size);
extern void ADC_Init_E(void);
extern uint16_t Get_AdcData(uint8_t ch);