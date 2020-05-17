#ifndef BUZZER__H
#define BUZZER__H

#include "bsp.h"

#define BEEP PAout(5)

void bspBuzzerInit(void);

void bspBuzzerFlash(void);

/*蜂鸣器跳变周期*/
/*最高频率 500Hz*/
/*最短周期 2ms*/

void bspBuzzerFreq(uint16_t  _usPeriod,uint16_t _usTotalPeriod);

/*蜂鸣器处理函数*/

void bspBeepHandler(void);

/*蜂鸣器周期使能*/

void bspBeepEnable(void);

/*蜂鸣器周期失能*/

void bspBeepDisable(void);

#endif
