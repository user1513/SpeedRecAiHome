#include "bsp_buzzer.h"


typedef struct tagBuzzer_T
{
    /* data */
    uint16_t usPeriod;          /*蜂鸣器单次翻转时间*/
    uint16_t usTotalPeriod;     /*蜂鸣器总响应时间*/
    uint16_t usBuzzerState;     /*蜂鸣器状态*/
    uint16_t usBuzrPerTmp;      /*蜂鸣器临时变量*/
    uint16_t usBuzrTotalPerTmp;      /*蜂鸣器临时变量*/
}Buzzer_T;

Buzzer_T tBuzzer = {0, 0, 0, 0};     /*定义蜂鸣器结构体*/

/*蜂鸣器初始化*/

void bspBuzzerInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOF时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//LED0和LED1对应IO口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
	
	GPIO_SetBits(GPIOA,GPIO_Pin_5);//GPIOF9,F10设置高，灯灭

}

/*蜂鸣器跳变*/

void bspBuzzerFlash(void)
{
    BEEP = (BEEP) ? 0 : 1;
}

/*蜂鸣器跳变周期*/
/*最高频率 500Hz*/
/*最短周期 2ms*/

void bspBuzzerFreq(uint16_t  _usPeriod,uint16_t _usTotalPeriod)

{
    tBuzzer.usPeriod = _usPeriod;

    tBuzzer.usTotalPeriod = _usTotalPeriod;
}

void bspBeepHandler(void)
{
    if(tBuzzer.usBuzzerState)                       /*蜂鸣器使能*/
    {
        tBuzzer.usBuzrPerTmp++;                      /*蜂鸣器翻转周期计数器*/

        if( tBuzzer.usBuzrPerTmp> tBuzzer.usPeriod)      
        {
            tBuzzer.usBuzrPerTmp = 0;

            bspBuzzerFlash();
        }
    
        if(tBuzzer.usTotalPeriod != 0xffff)         /*usTotalPeriod变量为0XFFFF代表蜂鸣器不会停止*/
        {
            tBuzzer.usBuzrTotalPerTmp++;               /*蜂鸣器总翻转周期计数器*/

            if(tBuzzer.usBuzrTotalPerTmp > tBuzzer.usTotalPeriod)       
            {
                tBuzzer.usBuzzerState = 0;              /*蜂鸣器标志位清零*/

                BEEP = 0;                               /*蜂鸣器失能*/

                //EventStopA(1);
            }  
        }      
    }
}

/*蜂鸣器周期使能*/

void bspBeepEnable(void)
{
    tBuzzer.usBuzzerState = 1;

    tBuzzer.usBuzrTotalPerTmp = 0;
}

/*蜂鸣器周期失能*/

void bspBeepDisable(void)
{
    tBuzzer.usBuzzerState = 0;              /*蜂鸣器标志位清零*/
            
    BEEP = 0;                               /*蜂鸣器失能*/
}

