#ifndef _BSP_PWM_H
#define _BSP_PWM_H
#include "sys.h"

//需要配合TIM_SetCompare1()使用
void TIM2_PWM_Init(u32 arr,u32 psc);

#endif
