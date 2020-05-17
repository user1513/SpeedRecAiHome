#ifndef __BSP_SPI_H
#define __BSP_SPI_H
#include "sys.h"
			  	    													  
void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度   
//void SPI2_ReadWriteByte(u16 TxData);//SPI总线读写一个字节
u16 SPI1_ReadWriteByte(u16 TxData);
//u8 SPI2_ReadWriteByte(u8 TxData);
		 
#endif
