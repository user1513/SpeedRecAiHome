#ifndef __BSP_ADC_H
#define __BSP_ADC_H	
#include "sys.h" 

 							   
void bsp_adc_init(void); 				//ADC通道初始化
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
#endif 















