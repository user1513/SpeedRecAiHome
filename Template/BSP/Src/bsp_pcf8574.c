#include "bsp_pcf8574.h"

/*定义bsp_swi2c.h内的Swi2c_T结构体变量*/

static void bsp_pcf8974x_out(void);
static void bsp_pcf8974x_in(void);

#if PCF8574_INT_STATE
static void pcf8974x_int_init(void);
static void bsp_pcf8974x_int_init(void);
#endif

Swi2c_T t_pcf8574_swi2c ={
	(unsigned long *)&PCF8974X_I2C_SCL,
	(unsigned long *)&PCF8974X_I2C_SDA_OUT,
	(unsigned long *)&PCF8974X_I2C_SDA_IN,
	bsp_pcf8974x_in,
	bsp_pcf8974x_out
};


void bsp_pcf8974x_io_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);//使能GPIOA时钟

	//GPIOA3,4初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIO

    GPIO_SetBits(GPIOD, GPIO_Pin_0 | GPIO_Pin_1);//GPIOA3,4置高

    #if PCF8574_INT_STATE

    pcf8974x_int_init();
    
    #endif

}

static void bsp_pcf8974x_out(void)
{
    PCF8974X_IO_OUT()
}

static void bsp_pcf8974x_in(void)
{
    PCF8974X_IO_IN()
}

static void bsp_pcf8974x_header(uint8_t _ucDeviceNo,uint8_t _ucWR)

{									
	swi2cSendByte(&t_pcf8574_swi2c, PCF8574X_ADDR +  ((_ucDeviceNo & 0x07) << 1) + (_ucWR & (0x01)));/*发送sgp30头部信息*/
 
	swi2cWaitAck(&t_pcf8574_swi2c, 1);									/*等待ack*/
}

/*注意_ucBitNum从0开始到7结束*/
void bsp_pcf8974x_bit_set(uint8_t _ucDeviceNo,uint8_t _ucBitNum, uint8_t level)
{
	uint8_t tmp = bsp_pcf8974x_receive(_ucDeviceNo);
	if(level)
		tmp |= 1 << _ucBitNum;
	else
		tmp &= ~(1 << _ucBitNum);
		
	bsp_pcf8974x_test(_ucDeviceNo, tmp);
}

void bsp_pcf8974x_test(uint8_t _ucDeviceNo, uint8_t _ucData)
	
{
    /*软件i2c开始信号*/
    swi2cStart(&t_pcf8574_swi2c);

    /*写pcf8974x的I2C头部信号*/
    bsp_pcf8974x_header(_ucDeviceNo, 0);

    /*发送数据*/
    swi2cSendByte(&t_pcf8574_swi2c, _ucData);

    /*等待ack*/
    swi2cWaitAck(&t_pcf8574_swi2c, 1);									

    /*软件i2c停止信号*/
    swi2cStop(&t_pcf8574_swi2c);
}

/*获取pcf8974x 的I2c数据*/

uint8_t bsp_pcf8974x_receive(uint8_t _ucDeviceNo) 
{
	uint8_t ucData = 0x00;
    /*软件i2c开始信号*/
    swi2cStart(&t_pcf8574_swi2c);

    /*写pcf8974x的I2C头部信号*/
    bsp_pcf8974x_header(_ucDeviceNo, 1);
	
	/*读取i2c数据*/
	ucData = swi2cReceiveByte(&t_pcf8574_swi2c);
	
    /*等待ack*/
    swi2cAck(&t_pcf8574_swi2c, 1);									

    /*软件i2c停止信号*/
    swi2cStop(&t_pcf8574_swi2c);
	
	return ucData;
}

#if PCF8574_INT_STATE

//外部中断2服务程序
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line6) == SET)
    {
     	uint8_t ucData = 0;

		ucData = bsp_pcf8974x_receive(0x01);	
		
		printf("bsp_pcf8974x_receive = %x\n", ucData);

	    EXTI_ClearITPendingBit(EXTI_Line6);//清除LINE2上的中断标志位 
    }
}

static void bsp_pcf8974x_int_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

	//GPIOA3,4初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
}
 
//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
static void pcf8974x_int_init(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);//PA6 连接到中断线6

    bsp_pcf8974x_int_init();

	/* 配置EXTI_Line0 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断6
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
}

#endif




