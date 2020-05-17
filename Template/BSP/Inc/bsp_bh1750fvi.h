#ifndef _BH1750FVI_H
#define _BH1750FVI_H

#include "bsp.h"

/*注意bh1750工作电压2.4~3.6v*/

#define BH1750FVI_LOW_LEVEL_ADDR    0X46                    /*bh1750fvi<--->ADD引脚接低电平时候的I2C地址 注意最低位为读写状态位*/
#define BH1750FVI_HIGH_LEVEL_ADDR   0XB8                    /*bh1750fvi<--->ADD引脚接高电平时候的I2C地址 注意最低位为读写状态位*/

/*按照ADDR的电平更换设备地址*/
#define BH1750FVI_DEVICE_ADDR   BH1750FVI_LOW_LEVEL_ADDR    /*使用的设备地址*/

#define BH1750FVI_H_RESOLUTION_MEASURE_TIME  120            /*bh1750fviH分辨率模式测量时间间隔*/
#define BH1750FVI_L_RESOLUTION_MEASURE_TIME  16             /*bh1750fviH分辨率模式测量时间间隔*/

/*定义几个重要的功能代码*/
#define BH1750FVI_POWER_OFF         0X00                    /*设备断电*/
#define BH1750FVI_POWER_ON          0X01                    /*设备上电*/
#define BH1750FVI_H_RESOLUTION      0X10                    /*连续H分辨率模式*/
#define BH1750FVI_H_RESOLUTION_2    0X11                    /*连续H分辨率模式2*/
#define BH1750FVI_L_RESOLUTION      0X13                    /*连续L分辨率模式*/
#define BH1750FVI_ONLY_H_RESOLUTION      0X20               /*单次H分辨率模式，测量后自动设置为断电模式*/
#define BH1750FVI_ONLY_H_RESOLUTION_2    0X21               /*单次H分辨率模式2，测量后自动设置为断电模式*/
#define BH1750FVI_ONLY_L_RESOLUTION      0X23               /*单次L分辨率模式，测量后自动设置为断电模式*/

//IO口引脚宏定义
#define BH1750FVI_I2C_SCL       PDout(15) 
#define BH1750FVI_I2C_SDA_IN    PCin(7)
#define BH1750FVI_I2C_SDA_OUT   PCout(7)


//IO方向设置
#define BH1750FVI_IO_IN()  {GPIOC->MODER&=(~((uint32_t)3<<(7*2)));GPIOC->MODER|=((uint32_t)0<<(7*2));}	//PA4输入模式
#define BH1750FVI_IO_OUT() {GPIOC->MODER&=(~((uint32_t)3<<(7*2)));GPIOC->MODER|=((uint32_t)1<<(7*2));} 	//PA4输出模式


/*应用函数*/
void bsp_bh1750fvi_io_init(void);                           /*bh1750fvi的IO引脚初始化*/  

uint16_t bh1750fvi_send_measure(uint8_t ucMode);            /*bh1750fvi开始测量，并且获取数据*/

#endif


