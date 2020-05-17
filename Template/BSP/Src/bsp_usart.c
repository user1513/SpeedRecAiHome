#include "bsp_usart.h"
#include "semphr.h"

extern SemaphoreHandle_t xUsartParseSemaphoreHandle;

void bspUsartInit(uint32_t bound){
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART1时钟
 
	//串口2对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA10复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_USART2); //GPIOA9复用为USART1	
	//USART2端口配置
    //除了复用ADC和DAC这两个外设时的mode选模拟输入外，其余外设的mode全部选AF
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2与GPIOA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2，PA3

    //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_CTS;//使能发送数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART2, &USART_InitStructure); //初始化串口2

	//USART_ClearFlag(USART1, USART_FLAG_TC);
    //当使用DMA接收时可以不使用接收中断,直接使能IDLE空闲中断,这样可以实现接收任意长度数据
	//USART_ITConfig(USART2,  USART_IT_RXNE, ENABLE);//开启相关中断	 
	USART_ITConfig(USART2,  USART_IT_IDLE, ENABLE);//开启相关中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口2中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=8;//抢占优先级7
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、

	USART_Cmd(USART2, ENABLE);  //使能串口2 
}


void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Res = Res;;
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)  //接收中断
	{
		Res = USART2->DR; 
		Res = Res;
		usartReadStart(swich_current_read_point(), 100);
		
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		xSemaphoreGiveFromISR(xUsartParseSemaphoreHandle, &xHigherPriorityTaskWoken);

		portYIELD_FROM_ISR( xHigherPriorityTaskWoken);		 
	} 
} 

