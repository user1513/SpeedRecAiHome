#include "bsp_bh1750fvi.h"
#include "bsp_swi2c.h"

static void bsp_bh1750fvi_out(void);

static void bsp_bh1750fvi_in(void);

/*发送bh1750fvi的I2C头部*/
static void bh1750fvi_send_header(uint8_t _ucWR);

/*发送bh1750fvi的I2C命令*/
static void bh1750fvi_send_cmd(uint8_t _ucCmd);

/*发送bh1750fvi的设备上电*/
static void bh1750fvi_send_power_on(void);

/*发送bh1750fvi的I2C数据读取*/
static void bh1750fvi_read_data(uint8_t* _ucTable, uint8_t length);

Swi2c_T t_bh1750fvi_swi2c ={
	(unsigned long *)&BH1750FVI_I2C_SCL,
	(unsigned long *)&BH1750FVI_I2C_SDA_OUT,
	(unsigned long *)&BH1750FVI_I2C_SDA_IN,
	(I2C_FUNC)bsp_bh1750fvi_in,
	(I2C_FUNC)bsp_bh1750fvi_out
};


void bsp_bh1750fvi_io_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOC,GPIOD时钟

	//GPIOA3,4初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;//LED0和LED1对应IO口
    GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO
    
    GPIO_SetBits(GPIOC, GPIO_Pin_7);//GPIOC7置高   
    GPIO_SetBits(GPIOD, GPIO_Pin_15);//GPIOC7置高

    bh1750fvi_send_power_on();

}


static void bsp_bh1750fvi_out(void)
{
    BH1750FVI_IO_OUT()
}

static void bsp_bh1750fvi_in(void)
{
    BH1750FVI_IO_IN()
}



/*发送bh1750fvi的I2C头部*/
static void bh1750fvi_send_header(uint8_t _ucWR)

{
    swi2cStart(&t_bh1750fvi_swi2c);											        /*I2C开始信号*/

	swi2cSendByte(&t_bh1750fvi_swi2c, BH1750FVI_DEVICE_ADDR + _ucWR);               /*发送BH1750FVI头部信息*/
 
	swi2cWaitAck(&t_bh1750fvi_swi2c, 1);									        /*等待BH1750FVI的ack*/
}

/*发送bh1750fvi的I2C命令*/
static void bh1750fvi_send_cmd(uint8_t _ucCmd)

{
	swi2cSendByte(&t_bh1750fvi_swi2c, _ucCmd);                                      /*发送BH1750FVI头部信息*/
 
	swi2cWaitAck(&t_bh1750fvi_swi2c, 1);									        /*等待BH1750FVI的ack*/
}

/*发送bh1750fvi的设备上电*/
static void bh1750fvi_send_power_on(void)
{
    bh1750fvi_send_header(0);                                                      /*发送头部*/

    bh1750fvi_send_cmd(BH1750FVI_POWER_OFF);                                       /*发生上电命令*/     

    swi2cStop(&t_bh1750fvi_swi2c);
}

/*发送bh1750fvi的I2C数据读取*/
static void bh1750fvi_read_data(uint8_t* _ucTable, uint8_t length)

{
	for(int i = 0; i < length; i++)
	{
		_ucTable[i] = swi2cReceiveByte(&t_bh1750fvi_swi2c);						/*读取sgp30的信息*/

		swi2cAck(&t_bh1750fvi_swi2c, i < (length - 1) ? 1 : 0);						/*master发送ack*/
	}
								
}

/*注意如果发送的命令带单次，需要每次测量前先设备上电*/
uint16_t bh1750fvi_send_measure(uint8_t ucMode)

{
    if(ucMode & 0x20)                                                               /*判断是否是单次测量*/
        bh1750fvi_send_power_on();                                                  /*设备上电*/

    bh1750fvi_send_header(0);                                                       /*发送头部*/
    
    bh1750fvi_send_cmd(ucMode);                                                     /*发送测量模式*/

    swi2cStop(&t_bh1750fvi_swi2c);                                                  /*停止I2C*/

    if(ucMode & 0x03)                                                               /*判断分辨率测量*/
        delay_ms(BH1750FVI_L_RESOLUTION_MEASURE_TIME);  
    else
        delay_ms(BH1750FVI_H_RESOLUTION_MEASURE_TIME); 
    
    bh1750fvi_send_header(1);                                                       /*发送头部*/                                                                   

    uint8_t ucaTmp[2] = {0};

    bh1750fvi_read_data(ucaTmp, 2);                                                 /*读取两个数据*/

    swi2cStop(&t_bh1750fvi_swi2c);                                                  /*停止I2C*/

    return ((ucaTmp[0] << 8) + ucaTmp[1]) / 1.2;                                    /*返回测量值单位lx*/
}
