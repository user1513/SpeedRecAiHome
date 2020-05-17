#ifndef _DSP_DHTxx_H
#define _DSP_DHTxx_H

#include "bsp.h"
#define DHT11       1
#define DHT22       2
#define DHTXX_DEVICE DHT22    //不同的器件进行不同的裁剪


#define DHTXX_RCC_AHB1Periph_GPIOx 	RCC_AHB1Periph_GPIOC
#define DHTXX_GPIOx 				GPIOC
#define DHTXX_GPIO_Pin_x 			GPIO_Pin_1
#define DHTXX_GPIO_Pin_num 			1
#define DHTXX_WRITE_DATA 			PCout(1)
#define DHTXX_READ_DATA 			PCin(1)
#define DHTXX_IO_IN(GPIOx,GPIO_Pin) {GPIOx->MODER &= ~(3 << (GPIO_Pin*2)); GPIOx->MODER|= (0 << (GPIO_Pin*2));}

#define DHTXX_IO_OUT(GPIOx,GPIO_Pin) {GPIOx->MODER &= ~(3 << (GPIO_Pin*2)); GPIOx->MODER|= (1 <<  (GPIO_Pin*2));}

//reg:dht_table[0] = hum,dh_table[1] = temp 
//return:0正常,非零读取失败
uint8_t DHTxx_Get_Data(double * DHT_Tab); 

void vInstability_Period(uint32_t nms);

//dht22 io 初始化
void vDhtxx_Init(void);




#endif

