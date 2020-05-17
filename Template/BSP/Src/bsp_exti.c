#include "bsp_exti.h"
#include "FreeRTOS.h"
#include "semphr.h"			/*用于信号量*/

extern SemaphoreHandle_t xKeySemaphoreHandle;

//外部中断15服务程序
void EXTI15_10_IRQHandler(void)
{
	 if((EXTI_GetITStatus(EXTI_Line15) == SET) || (EXTI_GetITStatus(EXTI_Line10) == SET))
	 {
		DISABLE_EXIT_LINE(EXTI_Line15);
		DISABLE_EXIT_LINE(EXTI_Line10);
		BaseType_t xHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(xKeySemaphoreHandle, &xHigherPriorityTaskWoken);
		EXTI_ClearITPendingBit(EXTI_Line15);//清除LINE2上的中断标志位 
		EXTI_ClearITPendingBit(EXTI_Line10);//清除LINE2上的中断标志位 
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken);	 
	 }
}
 
//外部中断初始化程序
//初始化PE2~4,PA0为中断输入.
void bspExitInit(void)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);//使能SYSCFG时钟

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource15);//PA6 连接到中断线6

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource10);//PA6 连接到中断线6
	/* 配置EXTI_Line15 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;//LINE15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line10;//LINE15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿触发 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能LINE0
	EXTI_Init(&EXTI_InitStructure);//配置

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//外部中断15
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x06;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);//配置
	
}












