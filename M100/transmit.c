//Nrf24l01 transmit mode test
//MCU AT89S52


#include <reg52.h>
#include <intrins.h>
#include "api.h"
#include "nrf24l01.h"

/**************************************************
Function: main();

/**************************************************/
void main(void)
{
	while(1)
	{	L2=0;
		NRF24L01_Config();
		delay_ms(100);

	 	if(k1==0)  //k1����ʱʹ��
		{  	delay_ms(5);
			if(k1==0)
			{
				 L2=1;
		 		 CE=1;
			   

			}
				while(!k1);
				 delay_ms(5);
				 while(!k1);	   //��������ʱ����
				NRF24L01_TxPacket(tab);
		
				check_ACK();//����Ӧ���źż�⣬LED��˸��־���ͳɹ�

				SPI_RW_Reg(WRITE_REG+STATUS,0xff);//���жϱ�־		  
	

		}

		//����K2ʱ�������
		if(k2==0)  //k1����ʱʹ��
		{  	delay_ms(5);
			if(k2==0)
			{
				 L2=1;
		 		 CE=1;
			   
	  

			}
			   	while(!k2);
				 delay_ms(5);
				 while(!k2);	   //��������ʱ����
				NRF24L01_TxPacket1(tab1);
		
				check_ACK();//����Ӧ���źż�⣬LED��˸��־���ͳɹ�

				SPI_RW_Reg(WRITE_REG+STATUS,0xff);//���жϱ�־	
		}
	
		//����K3ʱ�Ĳ���
		if(k3==0)  //k3����ʱʹ��
		{  	delay_ms(5);
			if(k3==0)
			{
				 L2=1;
		 		 CE=1;
			   
		  

			}
				while(!k3);
				 delay_ms(5);
				 while(!k3);	   //��������ʱ����
				NRF24L01_TxPacket2(tab2);
	
				check_ACK();//����Ӧ���źż�⣬LED��˸��־���ͳɹ�

				SPI_RW_Reg(WRITE_REG+STATUS,0xff);//���жϱ�־
		}


	}



	   

}


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




