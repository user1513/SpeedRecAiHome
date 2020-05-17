#include "bsp_encoder.h"
#include "stm32f4xx_tim.h"
void bsp_encoder_init(void)
{
    GPIO_InitTypeDef GPIO_InitTypeStruct;
    TIM_ICInitTypeDef TIM_ICInitTypeStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitTypeStruct;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD , ENABLE);//使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); //GPIOD12复用为TIM_CH1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); //GPIOD13复用为TIM_CH1

    GPIO_InitTypeStruct.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_12;//LED0和LED1对应IO口
	GPIO_InitTypeStruct.GPIO_Mode = GPIO_Mode_AF;//普通输出模式
	GPIO_InitTypeStruct.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
	GPIO_InitTypeStruct.GPIO_PuPd = GPIO_PuPd_UP;//上拉D
	GPIO_Init(GPIOD, &GPIO_InitTypeStruct);//初始化GPIO
	
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitTypeStruct);
    TIM_TimeBaseInitTypeStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitTypeStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitTypeStruct.TIM_Period = 65535;
	TIM_TimeBaseInitTypeStruct.TIM_Prescaler = 0; 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitTypeStruct);
	
  
	TIM_ICStructInit(&TIM_ICInitTypeStruct); 
    TIM_ICInitTypeStruct.TIM_ICFilter = 0x0a;
    TIM_ICInit(TIM4, &TIM_ICInitTypeStruct);

    TIM_ICInitTypeStruct.TIM_Channel = TIM_Channel_2;
//    TIM_ICInitTypeStruct.TIM_ICFilter = 0x0a;
//    TIM_ICInitTypeStruct.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
//    TIM_ICInitTypeStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//    TIM_ICInitTypeStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInit(TIM4, &TIM_ICInitTypeStruct);

	
    TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge, TIM_ICPolarity_BothEdge);
	
	TIM_SetCounter(TIM4,0); //TIM3->CNT=0
	
    TIM_Cmd(TIM4 , ENABLE);
}
