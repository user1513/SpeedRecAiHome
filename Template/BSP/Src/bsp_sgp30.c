#include "bsp_sgp30.h"
#include "bsp.h"
#include "float.h"
#include "math.h"
/*定义bsp_swi2c.h内的Swi2c_T结构体变量*/

Swi2c_T t_sgp30_swi2c ={
	(unsigned long *)&SGP30_I2C_SCL,
	(unsigned long *)&SGP30_I2C_SDA_OUT,
	(unsigned long *)&SGP30_I2C_SDA_IN,
	bsp_sgp30_in,
	bsp_sgp30_out
};

/*定义crc.h内的CRC_8结构体变量*/

CRC_8 g_sgp30_crc8 = {0x31, 0xff, 0x00, FALSE, FALSE}; 


// 5 10 15 20 25 30 35 40 45 50 55 60 65 70 75 80 85 90 95 100
//单位:g/m3
//横坐标:相对湿度
//纵坐标:温度
//大气压:1bar

const float Absolute_humidity_table[][20] = {
	{0.34, 0.68, 1.02, 1.36, 1.70, 2.04, 2.38, 2.72, 3.06, 3.40, 3.73, 4.07, 4.41, 4.75, 5.09, 5.43, 5.77, 6.11, 6.45, 6.79},
	{0.47, 0.94, 1.41, 1.88, 2.35, 2.82, 3.29, 3.76, 4.23, 4.70,5.16,5.63,6.10,6.57,7.04,7.51,7.98,8.45,8.92,9.39},
	{0.64,1.28,1.92,2.56,3.21,3.85,4.49,5.13,5.77,6.41,7.05,7.69,8.33,8.97,9.62,10.26,10.90,11.54,12.18,12.82},
	{0.86,1.73,2.59,3.45,4.32,5.18,6.04,6.91,7.77,8.64,9.50,10.36,11.23,12.09,12.95,13.82,14.68,15.54,16.41,17.27},
	{1.15,2.30,3.45,4.60,5.75,6.90,8.05,9.20,10.35,11.51,12.66,13.81,14.96,16.11,17.26,18.41,19.56,20.71,21.86,23.01},
	{1.52,3.03,4.55,6.06,7.58,9.09,10.61,12.12,13.64,15.16,16.67,18.19,19.70,21.22,22.73,24.25,25.76,27.28,28.79,30.31},
	{1.98,3.95,5.93,7.90,9.88,11.85,13.83,15.80,17.78,19.76,21.73,23.71,25.68,27.66,29.63,31.61,33.58,35.56,37.53,39.51},
	{2.55,5.10,7.65,10.20,12.75,15.30,17.85,20.40,22.95,25.50,28.05,30.60,33.15,35.70,38.25,40.80,43.35,45.90,48.45,51.00},
	{3.26,6.52,9.78,13.04,16.30,19.56,22.82,26.08,29.34,32.61,35.87,39.13,42.39,45.65,48.91,52.17,55.43,58.69,61.95,65.21},
	{4.13,8.27,12.40,16.53,20.66,24.80,28.93,33.06,37.19,41.33,45.46,49.59,53.72,57.86,61.99,66.12,70.25,74.39,78.52,82.65},
	{5.19,10.39,15.58,20.78,25.97,31.17,36.36,41.56,46.75,51.95,57.14,62.33,67.53,72.72,77.92,83.11,88.31,93.50,98.70,103.89},
	{6.48,12.95,19.43,25.91,32.39,38.86,45.34,51.82,58.29,64.77,71.25,77.72,84.20,90.68,97.16,103.63,110.11,116.59,123.06,129.54},
	{8.02,16.03,24.05,32.06,40.08,48.09,56.11,64.12,72.14,80.15,88.17,96.18,104.20,112.21,120.23,128.24,136.26,144.27,152.29,160.30},
	{9.85,19.69,29.54,39.39,49.24,59.08,68.93,78.78,88.62,98.47,108.32,118.16,128.01,137.86,147.71,157.55,167.40,177.25,187.09,196.94},
	{12.02,24.03,36.05,48.06,60.08,72.09,84.11,96.12,108.14,120.16,132.17,144.19,156.20,168.22,180.23,192.25,204.26,216.28,228.29,240.31},
	{14.57,29.13,43.70,58.27,72.83,87.40,101.97,116.53,131.10,145.67,160.23,174.80,189.36,203.93,218.50,233.06,247.63,262.20,276.76,291.33},
	{17.55,35.10,52.65,70.20,87.75,105.29,122.84,140.39,157.94,175.49,193.04,210.59,228.14,245.69,263.24,280.78,298.33,315.88,333.43,350.98},
	{21.02,42.04,63.05,84.07,105.09,126.11,147.13,168.14,189.16,210.18,231.20,252.22,273.23,294.25,315.27,336.29,357.31,378.32,399.34,420.36},
	{25.03,50.06,75.09,100.12,125.15,150.18,175.21,200.24,225.27,250.30,275.33,300.36,325.39,350.42,375.45,400.48,425.51,450.54,475.57,500.60},
	{29.65,59.30,88.94,118.59,148.24,177.89,207.54,237.18,266.83,296.48,326.13,355.78,385.42,415.07,444.72,474.37,504.02,533.66,563.31,592.96},
};


static void bsp_sgp30_out(void);

static void bsp_sgp30_in(void);

/*sgp30设置读写状态头部函数*/
static void bsp_sgp30_header(uint8_t _ucWR);
/*发送sgp30 2字节命令*/
static void bsp_sgp30_comm(uint16_t _usComm);
/*发送sgp30 数据*/
static void bsp_sgp30_write_data(uint8_t* _ucTable, uint8_t length);
/*读取sgp30 数据*/
static void bsp_sgp30_read_data(uint8_t* _ucTable, uint8_t length);

static void bsp_sgp30_sensor_init(void);

void bsp_sgp30_io_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

	//GPIOA3,4初始化设置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;//LED0和LED1对应IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

    GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_4);//GPIOA3,4置高

	bsp_sgp30_sensor_init();
}


static void bsp_sgp30_out(void)
{
    SGP30_IO_OUT()
}

static void bsp_sgp30_in(void)
{
    SGP30_IO_IN()
}

/*sgp30设置读写状态头部函数*/

static void bsp_sgp30_header(uint8_t _ucWR)

{
	swi2cStart(&t_sgp30_swi2c);											/*I2C开始信号*/

	swi2cSendByte(&t_sgp30_swi2c, (SGP30_ADDRESS << 1) | (_ucWR & (0x01)));/*发送sgp30头部信息*/
 
	swi2cWaitAck(&t_sgp30_swi2c, 1);									/*等待sgp30的ack*/
}

/*发送sgp30 2字节命令*/

static void bsp_sgp30_comm(uint16_t _usComm)

{
	swi2cSendByte(&t_sgp30_swi2c, _usComm >> 8);						/*发送sgp30命令高八位信息*/

	swi2cWaitAck(&t_sgp30_swi2c, 1);									/*等待sgp30的ack*/
 
 	swi2cSendByte(&t_sgp30_swi2c, (uint8_t)_usComm);					/*发送sgp30命令低八位信息*/

	swi2cWaitAck(&t_sgp30_swi2c, 1);									/*等待sgp30的ack*/
}

/*发送sgp30 数据*/

static void bsp_sgp30_write_data(uint8_t* _ucTable, uint8_t length)

{
	for(int i = 0; i < length; i++)
	{
		swi2cSendByte(&t_sgp30_swi2c, _ucTable[i]);							/*发送sgp30命令高八位信息*/

		swi2cWaitAck(&t_sgp30_swi2c, 1);									/*等待sgp30的ack*/
	}

}

/*读取sgp30 数据*/

static void bsp_sgp30_read_data(uint8_t* _ucTable, uint8_t length)

{
	for(int i = 0; i < length; i++)
	{
		_ucTable[i] = swi2cReceiveByte(&t_sgp30_swi2c);						/*读取sgp30的信息*/

		swi2cAck(&t_sgp30_swi2c, i < (length - 1) ? 1 : 0);						/*master发送ack*/
	}
								
}

/*sgp30传感器状态初始化*/

static void bsp_sgp30_sensor_init(void)
{
	bsp_sgp30_header(0);

	bsp_sgp30_comm(INIT_AIR_QUALITY);

	swi2cStop(&t_sgp30_swi2c);
}

uint8_t g_ucaTable[6] = {0};


/******************************
**获取sgp30 MEASURE_AIR_QUALITY 的6位数据 
**6位数据数据格式
** xx xx xx xx xx xx
** co2
*********crc
************tvoc
*****************crc		
*******************************/

void bsp_sgp30_test(void)
{
	TIM_Cmd(TIM3,DISABLE); //失能定时器3

    bsp_sgp30_header(0);

	bsp_sgp30_comm(MEASURE_AIR_QUALITY);
	
	swi2cStop(&t_sgp30_swi2c);

	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	delay_ms(100);		/*需要一点时间的等待*/
	
	TIM_Cmd(TIM3,DISABLE); //失能定时器3

	bsp_sgp30_header(1);

	bsp_sgp30_read_data(g_ucaTable, 6);
	
	swi2cStop(&t_sgp30_swi2c);

	TIM_Cmd(TIM3,ENABLE); //使能定时器3

	/*检测crc检验值*/

	if((crc8(g_ucaTable, 2, &g_sgp30_crc8) != g_ucaTable[2]) ||
	   (crc8(g_ucaTable + 3, 2, &g_sgp30_crc8) != g_ucaTable[5]))
	{
		/*将数组清零*/
		memset(g_ucaTable, 0, 6);
	}
}


/*通过查表法获取绝对湿度*/
/*形参:*/
static float GetAbsolutehumidity(double _fTemp, double _fHum)
{
	/*定义行号*/
	uint8_t ucRow0 = 0xff;
	uint8_t ucRow1 = 0xff;
	
	/*定义行号*/
	uint8_t ucColumn0 = 0xff;
	uint8_t ucColumn1 = 0xff;
	
	/*查表法的范围温度(5~100℃),湿度(5~100%)*/
	
	if((_fTemp >= 5 && _fTemp <= 100) &&
		(_fHum >= 5 && _fHum <= 100))
	{
		//判断输入湿度是否是5的整数被
		if(fabs((_fHum / 5.0) - ((int)_fHum / 5)) < FLT_EPSILON)
		{
			ucColumn0 = (uint8_t) (_fHum / 5.0) - 1;
		}
		else
		{
			ucColumn0 = (uint8_t) (_fHum / 5.0) - 1;

			ucColumn1 = ucColumn0 + 1;
		}

		//判断输入温度是否是5的整数被
		if(fabs((_fTemp / 5.0) - ((int)_fTemp / 5)) < FLT_EPSILON)
		{
			ucRow0 = (uint8_t) (_fTemp / 5.0) - 1;
		}
		else
		{
			ucRow0 = (uint8_t) (_fTemp / 5.0) - 1;

			ucRow1 = ucRow0 + 1;
		}

		/*当输入的湿度,温度为5的整数被时,可以直接通过查表就可以得出绝对湿度*/

		if(ucColumn1 == 0xff && ucRow1 == 0xff)
		{
			return Absolute_humidity_table[ucRow0][ucColumn0];
		}

		/*当输入的温度为5的整数倍时,只需要构建二元一次函数得出绝对湿度*/

		if(ucRow1 == 0xff)
		{
			return ((Absolute_humidity_table[ucRow0][ucColumn1] -
					Absolute_humidity_table[ucRow0][ucColumn0]) / 
					5 *
					(_fHum - (int)(_fHum / 5) * 5) + Absolute_humidity_table[ucRow0][ucColumn0]);
		}

		/*当输入的相对湿度度为5的整数倍时,只需要构建二元一次函数得出绝对湿度*/

		if(ucColumn1 == 0xff)
		{
			
			return ((Absolute_humidity_table[ucRow1][ucColumn0] -
					Absolute_humidity_table[ucRow0][ucColumn0]) / 
					5 *
					(_fTemp - (((int)(_fTemp / 5)) * 5))) + Absolute_humidity_table[ucRow0][ucColumn0];
		}

		/*当均不满足上面条件时通过三次构建二元一次函数得出绝对湿度*/

		float tmp[2] = {0.0, 0.0};

		tmp[0] = ((Absolute_humidity_table[ucRow0][ucColumn1] -
				Absolute_humidity_table[ucRow0][ucColumn0]) / 
				5 *
				(_fHum - (int)(_fHum / 5) * 5) + Absolute_humidity_table[ucRow0][ucColumn0]);

		tmp[1] = ((Absolute_humidity_table[ucRow1][ucColumn1] -
				Absolute_humidity_table[ucRow1][ucColumn0]) / 
				5 *
				(_fHum - (int)(_fHum / 5) * 5) + Absolute_humidity_table[ucRow1][ucColumn0]);

		return ((tmp[1] - tmp[0]) / 
					5 *
				(_fTemp - (int)(_fTemp / 5) * 5)) + tmp[0];
	}

	return 0.0f;
}

/*定义i2c发送数组*/
uint8_t g_ucaI2cSend[3] = {0};

/*启动片上湿度补偿*/
void bsp_sgp30_humidity_compensation(double _fTemp, double _fHum)
{
	float fTmp = 0.0f; 
	
	
	fTmp = GetAbsolutehumidity(_fTemp, _fHum);

//	printf("GetAbsolutehumidity:%0.3f\n", fTmp);

	if(fTmp == 0)
		return;
	
	if(fTmp > 255)
		fTmp = 255; 
	/*获取整数部分*/
	g_ucaI2cSend[0] =  (int)fTmp;	
	
	/*获取小数部分,并且以256编码*/
	g_ucaI2cSend[1] = (fTmp - (int)fTmp) * 256;
	
	/*获取crc8校验*/
	g_ucaI2cSend[2] = crc8(g_ucaI2cSend, 2, &g_sgp30_crc8);
	
	bsp_sgp30_header(0);

	bsp_sgp30_comm(SET_HUMIDITY);
	
	bsp_sgp30_write_data(g_ucaI2cSend, 3);
	
	swi2cStop(&t_sgp30_swi2c);
	
}



