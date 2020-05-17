#ifndef _BSP_SGP30_H
#define _BSP_SGP30_H

#include "bsp.h"

//sgp30I2C地址
#define SGP30_ADDRESS 0x58 

#define INIT_AIR_QUALITY        0X2003          /*初始化SGP30*/
#define MEASURE_AIR_QUALITY     0X2008          /*SGP30开始测量*/
#define GET_BASELINE            0X2015          /*获取SGP30基准线*/
#define SET_BASELINE            0X201E          /*设置SGP30基准线*/
#define SET_HUMIDITY            0X2061          /*设置绝对湿度*/
//#define MEASURE_TEST            0X2032          /*运行片上自测试*/
#define GET_FEATURE_SET_VERSION 0X202F          /*获取功能集版本*/
#define MEASURE_RAW_SIGNALS     0X2050          /*测量的原始信号*/

//IO口引脚宏定义
#define SGP30_I2C_SCL       PAout(3) 
#define SGP30_I2C_SDA_IN    PAin(4)
#define SGP30_I2C_SDA_OUT   PAout(4)


//IO方向设置
#define SGP30_IO_IN()  {GPIOA->MODER&=~(3<<(4*2));GPIOA->MODER|=0<<4*2;}	//PA4输入模式
#define SGP30_IO_OUT() {GPIOA->MODER&=~(3<<(4*2));GPIOA->MODER|=1<<4*2;} 	//PA4输出模式
/*函数声明*/

void bsp_sgp30_io_init(void);

void bsp_sgp30_test(void);

void bsp_sgp30_out(void);

void bsp_sgp30_in(void);

void bsp_sgp30_humidity_compensation(double _fTemp, double _fHum);

#endif

