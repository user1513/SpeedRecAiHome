#include "hc595.h"
#include "delay.h"



void HC595_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_5|GPIO_Pin_4;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOD, &GPIO_InitStructure);					 
					
	Date_OE = 0;
	Date_SHCP = 0;
	Date_STCP = 0;
	Date_DS = 0;
}


u8 display[16]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};

void HC595TransmitData(void)
{
	u8 i,j;
	u8 data;
	for(j=0; j<17-4; j++)
	{
		data = ~display[j];	
		for(i=0;i<8;i++)
		{
			Date_DS = data>>7;	//从高位到低位
			data<<=1;
			Date_SHCP = 1;
			Date_SHCP = 0;
		}		
	}
	for(j=0; j<4; j++)
	{
		data=display[j];
		for(i=0;i<8;i++)
		{
			Date_DS = data>>7;	//从高位到低位
			data<<=1;
			//Date_DS = 1;	//从高位到低位
			Date_SHCP = 1;
			Date_SHCP = 0;
		}		
	}
	
	//--输出--//
	Date_STCP = 0; //set dataline low
	Date_STCP = 1; //片选
	Date_STCP = 0; //set dataline low
}
