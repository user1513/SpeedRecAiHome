#ifndef _BSP_KEY_H
#define _BSP_KEY_H	 
#include "sys.h" 

#define config_USE_ZDYZ 0	/*使用正点原子f407板子*/
#define config_USE_DIY	1	/*使用非正点的板子*/

#define key_DEVICE_NAME config_USE_DIY

#if key_DEVICE_NAME == config_USE_ZDYZ
/*下面的方式是通过直接操作库函数方式读取IO*/
#define KEY0 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2) //PE2
#define WK_UP 	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)	//PA0


/*下面方式是通过位带操作方式读取IO*/
/*
#define KEY0 		PEin(4)   	//PE4
#define KEY1 		PEin(3)		//PE3 
#define KEY2 		PEin(2)		//P32
#define WK_UP 	PAin(0)		//PA0
*/



#else

#define KEY 	PAin(15)
#define KEY1 	PAin(10)
#endif

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define KEY2_PRES	3
#define WKUP_PRES   4

void bspKeyInit(void);			//IO初始化

u8 KEY_Scan(u8);  				//按键扫描函数	

u8 uasrt_key_send(u8 _ucKey);	//通过串口模拟发送按键值

u8 uasrt_key_get(void);			//获取串口发送按键值

#endif
