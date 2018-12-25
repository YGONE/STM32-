//Nrf24l01 receive mode test
//MCU AT89S52

#include <reg52.h>
#include <intrins.h>
#include "api.h"
#include "nrf24l01.h"
#include"1602.h"

uint i;
uchar flag;

/**************************************************
Function: main();

/**************************************************/

void main(void)
{
	int j=0;
    
	LCD_Init();
	L1=0;//LED亮标志开始工作
    NRF24L01_Config();
	delay_ms(100);
    NRF24L01_RxPacket(rx_buf);
   	delay_ms(100);
	
	while(1)
	{
	  L1=0;
	  delay_ms(100);
	  L1=1;
	// check_ACK(); 
	   LCD_POS(0X40);
	 if(NRF24L01_RxPacket(rx_buf))
	 for(j=0;j<10;j++)
	 {
		 LCD_WRITE_DATA(rx_buf[j]); 
		  delay_ms(10); 
	   	 
	 }																					
	   
	} 
} 
  
/*
void main(void)
{
	int j=0;

    LCD_Init();
	L1=0;//LED亮标志开始工作
	RX_Mode();//接收模式 
	while(1)
	{																							
	   CE=0;
    
		sta=SPI_Read(READ_REG +STATUS);//检测状态寄存器状态

	  if(RX_DR)
	   	{
		
			SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);//读取接收缓冲区数值
		    flag=1;

		}
		if(MAX_RT)
		{
		 SPI_RW_Reg(FLUSH_TX,0);
		}
		SPI_RW_Reg(WRITE_REG+STATUS,sta);
		if(flag)
		{  
		    flag=0; 
		 	L1=1;//LED闪烁标志接收成功
			delay_ms(100);
			L1=0;
		  LCD_POS(0X40);
         for(i=0;i<10;i++)
		   {  
	  		LCD_WRITE_DATA(b[rx_buf[i]]);
	  	   }
		} 
		SPI_RW_Reg(WRITE_REG+STATUS,0xff);//清中断标志
	} 
}  */
  
/**************************************************
Function: delay_ms(unsigned int x)

/**************************************************/
void delay_ms(unsigned int x)
{
    unsigned int i,j;
    i=0;
    for(i=0;i<x;i++)
    {
       j=108;
       while(j--);
    }
}
/**************************************************/


