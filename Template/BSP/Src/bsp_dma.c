#include "bsp_dma.h"
#include "semphr.h"

/*dma模式*/
static uint8_t g_DmaMode = 0; 

extern QueueHandle_t xDmaModeMutexHandle;

static void bspDmaUsartSendCfg(uint8_t* buf0,uint16_t num);/*DMA串口发送配置函数*/

static void bspDmaUsartReadCfg(uint8_t* buf0,uint16_t num);/*DMA串口接收配置函数*/

static uint8_t g_DmaTable1[100];

static uint8_t g_DmaTable2[100];

static uint8_t* g_ucCurrentPoint = g_DmaTable1;

/*DMA串口初始化*/

void bspDmaUsartInit(void)
{
	bspDmaUsartSendCfg(NULL,0);
	
	bspDmaUsartReadCfg(g_ucCurrentPoint,100);
}

/*DMA串口发送配置函数*/

static void bspDmaUsartSendCfg(uint8_t* buf0,uint16_t num)
	
{

    NVIC_InitTypeDef   NVIC_InitStructure;

	DMA_InitTypeDef  DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//DMA1时钟使能 

    USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);	/*使能写串口dma*/
    
    DMA_Cmd(DMA1_Stream6,DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道0 SPI2_TX通道 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;//外设地址为:(u32)&SPI2->DR
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)buf0;//DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
    DMA_InitStructure.DMA_BufferSize = num;//数据传输量 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度：8位 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用正常模式 
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //不使用FIFO模式        
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//外设突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//存储器突发单次传输
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);//初始化DMA Stream
	
    DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);//开启传输完成中断
 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置
}


/*DMA串口接收配置函数*/

static void bspDmaUsartReadCfg(uint8_t* buf0,uint16_t num)
	
{

    NVIC_InitTypeDef   NVIC_InitStructure;

	DMA_InitTypeDef  DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//DMA1时钟使能 

    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);	/*使能写串口dma*/
    
    DMA_Cmd(DMA1_Stream5,DISABLE);

    DMA_InitStructure.DMA_Channel = DMA_Channel_4;  //通道0 SPI2_TX通道 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART2->DR;//外设地址为:(u32)&SPI2->DR
    DMA_InitStructure.DMA_Memory0BaseAddr = (u32)buf0;//DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式
    DMA_InitStructure.DMA_BufferSize = num;//数据传输量 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度：8位 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用正常模式 
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable; //不使用FIFO模式        
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//外设突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//存储器突发单次传输
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);//初始化DMA Stream
	
    DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE);//开启传输完成中断
 
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;//抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
    NVIC_Init(&NVIC_InitStructure);//配置
	
	DMA_Cmd(DMA1_Stream5,ENABLE);//开启DMA TX传输,开始发送数据	
}

/*USART数据发送开始*/
void usartSendStart(uint8_t* buf,uint16_t buf_count)
{   	  
    DMA_Cmd(DMA1_Stream6,DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream6, buf_count);
	DMA1_Stream6->M0AR = (uint32_t)buf;
	DMA_Cmd(DMA1_Stream6,ENABLE);//开启DMA TX传输,开始发送数据	
}

/*USART数据发送结束*/
void usartSendStop(void)
{   	  
    DMA_Cmd(DMA1_Stream6,DISABLE);
}

/*USART数据数据接收开始*/
void usartReadStart(uint8_t* buf,uint16_t buf_count)
{   	  
    DMA_Cmd(DMA1_Stream5,DISABLE);
    DMA_SetCurrDataCounter(DMA1_Stream5, buf_count);
	DMA1_Stream5->M0AR = (uint32_t)buf;
	DMA_Cmd(DMA1_Stream5,ENABLE);//开启DMA TX传输,开始发送数据	
}

/*USART数据接收停止*/
void usartReadStop(void)
{   	  
    DMA_Cmd(DMA1_Stream5,DISABLE);
}


void SetDmaStatus(uint8_t _ucMode)
	
{
	//xSemaphoreTake(xDmaModeMutexHandle, portMAX_DELAY);
	vTaskSuspendAll();
	
	g_DmaMode |= _ucMode;
	
	xTaskResumeAll();
	//xSemaphoreGive(xDmaModeMutexHandle);
}

uint8_t GetDmaStatus(void)
{
	return g_DmaMode;
}

//DMA1_Stream6中断服务函数
void DMA1_Stream6_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6)==SET)	//DMA1_Stream6,传输完成标志
	{ 
		DMA_ClearITPendingBit(DMA1_Stream6,DMA_IT_TCIF6);	//清除传输完成中断	
		
		uint8_t mode = GetDmaStatus();
		
		if(mode & 0x80)		/*清除发送标志*/
		{
			g_DmaMode &= ~0x80;
		}		
	}  											 
} 

//DMA1_Stream5中断服务函数
void DMA1_Stream5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_TCIF5)==SET)	//DMA1_Stream5,传输完成标志
	{ 
		DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_TCIF5);	//清除传输完成中断	
	}  											 
}

/*切换当前dma读地址*/
uint8_t* swich_current_read_point(void)
{
	g_ucCurrentPoint = (g_ucCurrentPoint == g_DmaTable1) ? g_DmaTable2 : g_DmaTable1;

	return g_ucCurrentPoint;	
}

/*获取当前串口读取完成的指针*/
uint8_t* read_current_finaliy_point(void)
{
	return (g_ucCurrentPoint == g_DmaTable1) ? g_DmaTable2 : g_DmaTable1;
}

