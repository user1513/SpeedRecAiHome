#include "bsp_dhtxx.h"

static void Io_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(DHTXX_RCC_AHB1Periph_GPIOx,ENABLE); //使能GPIOD的时钟
	
	GPIO_InitStructure.GPIO_Pin=DHTXX_GPIO_Pin_x;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;//输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;  //上拉输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz; //高速GPIO
	GPIO_Init(DHTXX_GPIOx,&GPIO_InitStructure);
	
	GPIO_SetBits(DHTXX_GPIOx,DHTXX_GPIO_Pin_x); //GPIOF 高电平
}

//不稳地期间
void vInstability_Period(uint32_t nms)
{
	delay_ms(nms);
}

void vDhtxx_Init(void)
{
	Io_Init();
}

//初始化默认 uState = 0 输出模式
//uState非零为输入模式
static void vSetIoState(uint8_t uState)
{
	if(uState)
	{
		DHTXX_IO_IN(DHTXX_GPIOx,DHTXX_GPIO_Pin_num)
	}
		
	else
	{
		DHTXX_IO_OUT(DHTXX_GPIOx,DHTXX_GPIO_Pin_num)
	}
		
}

//主机发送dht开始信号
static void vDHTxx_Start(void)
{
	vSetIoState(0); 
	DHTXX_WRITE_DATA = 0;
	delay_us(550);
	DHTXX_WRITE_DATA = 1;
	delay_us(20);
}

//输出非零表示接收应当失败
//输出零表示接收应答成功
//dht发送主机应当信号
static uint8_t uDHTxx_Answer(void)
{
	int i = 0;
	vSetIoState(0xff);
	while(DHTXX_READ_DATA != 0)
	{
		i++;
		delay_us(10);
		if(i >= 10)
		{
			return i;
		}
	}
	i = 0;
	while(DHTXX_READ_DATA != 1)
	{
		i++;
		delay_us(10);
		if(i >= 10)
		{
			return i;
		}
	}
	return 0;
}

//读取一个字节的数据
//返回值非零代表读取失败
//返回值零代表读取成功
static uint8_t uRead_Byte(uint8_t *Read_Byte)
{
	// i 返回值, j作为for的变量
	int i = 0, j = 0;
	uint8_t _read_byte = 0;
	
	while(DHTXX_READ_DATA != 0)
	{
		i++;
		delay_us(10);
		if(i >= 10)
		{
			return i;
		}
	}
	
	i = 0;
	for(j = 0; j < 8; j++)
	{
		while(DHTXX_READ_DATA != 1)
		{
			i++;
			delay_us(10);
			if(i >= 10)
			{
				return i;
			}
		}
		
		i = 0;
		
		while(DHTXX_READ_DATA == 1)
		{
			i++;
			delay_us(10);	
		}
		_read_byte <<= 1;
		if(i >= 5)
		{
			_read_byte |= 0x01;
		}	
		i = 0;
	}
	*Read_Byte = _read_byte;
	return 0;
}
uint8_t g_dhtxx_table[5];

static void DHTxx_Action(uint8_t *dhtxx_table)
{
	vDHTxx_Start();
	uDHTxx_Answer();
	uRead_Byte(dhtxx_table++);
	uRead_Byte(dhtxx_table++);
	uRead_Byte(dhtxx_table++);
	uRead_Byte(dhtxx_table++);
	uRead_Byte(dhtxx_table);
}

uint8_t DHTxx_Get_Data(double * DHTxx_Tab)
{
    u8 CheckVal = 0;
	int flag = 1;
	DHTxx_Action(g_dhtxx_table);
	CheckVal = g_dhtxx_table[0] + g_dhtxx_table[1] + g_dhtxx_table[2] + g_dhtxx_table[3];
	if(CheckVal == g_dhtxx_table[4])
	{
		#if DHTXX_DEVICE == DHT22
			DHTxx_Tab[0] = ((g_dhtxx_table[0] << 8 )+ g_dhtxx_table[1])/10.0;
			if(g_dhtxx_table[2]&0x80)
			{
				g_dhtxx_table[2] &= ~0x80; 
				flag = -1;
			}
			DHTxx_Tab[1] = flag * ((g_dhtxx_table[2] << 8 )+ g_dhtxx_table[3])/10.0;
		#elif DHTXX_DEVICE == DHT11
			DHTxx_Tab[0] = (dhtxx_table[0] ;
			if(g_dhtxx_table[2]&0x80)
			{
				g_dhtxx_table[2] &= ~0x80; 
				flag = -1;
			}
			DHTxx_Tab[1] = flag * dhtxx_table[2];
		#endif
			
		return 0;
	}
	return 0xff;
	
}



