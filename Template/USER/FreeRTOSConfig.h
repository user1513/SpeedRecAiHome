/*
 * FreeRTOS Kernel V10.3.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#include "sys.h"
#include "usart.h"
	
/* Ensure stdint is only used by the compiler, and not the assembler. */
#ifdef __ICCARM__
	#include <stdint.h>
	extern uint32_t SystemCoreClock;
#endif


	
/*使能抢占式调度器*/
#define configUSE_PREEMPTION			1
#define configUSE_IDLE_HOOK				0
#define configUSE_TICK_HOOK				0
/*系统主频168MHz*/
#define configCPU_CLOCK_HZ				((unsigned long) 168000000 )
/*时钟节拍1KHz,即1ms*/
#define configTICK_RATE_HZ				( ( TickType_t ) 1000 )
/*定义可供用户使用的最大优先级数，如果这个定义的是5，
那么用户可以使用的优先级号是0,1,2,3,4， 不包含5，*/
/*优先级数越大优先级越高*/
#define configMAX_PRIORITIES			( 16 )

#define configMINIMAL_STACK_SIZE		( ( unsigned short ) 130 )
#define configTOTAL_HEAP_SIZE			( ( size_t ) ( 40 * 1024 ) )
#define configMAX_TASK_NAME_LEN			( 16 )
#define configUSE_TRACE_FACILITY		1
#define configUSE_16_BIT_TICKS			0
#define configIDLE_SHOULD_YIELD			0

#define configSUPPORT_DYNAMIC_ALLOCATION 1

/*使用互斥信号量,是二值信号量的子集*/
#define configUSE_MUTEXES				1

/*设置成1使能队列集功能（可以阻塞、挂起到多个队列和信号量），设置成0取消队列集功能*/
#define configUSE_QUEUE_SETS			1


#define configQUEUE_REGISTRY_SIZE		8
#define configCHECK_FOR_STACK_OVERFLOW	0
#define configUSE_RECURSIVE_MUTEXES		1
#define configUSE_MALLOC_FAILED_HOOK	0
#define configUSE_APPLICATION_TASK_TAG	0
#define configUSE_COUNTING_SEMAPHORES	1
#define configGENERATE_RUN_TIME_STATS	0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES 		0
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/* Software timer definitions. */
#define configUSE_TIMERS				1
#define configTIMER_TASK_PRIORITY		( 2 )
#define configTIMER_QUEUE_LENGTH		10
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	0
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/*
 * Cortex-M内核使用8bit来配置优先级，但是STM32只使用了高4bit，数值越小，优先级越高。
 * 在往寄存器里面写数值配置的时候，是按照8bit来写的，所以真正写的时候需要经过转换，公式为：
 * ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff)，其中的priority就是我们配置的真正的优先级
 */
#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4        /* 15 priority levels */
#endif

/*============================================== SysTick中断优先级配置 ============================================*/
/*
 * 在往寄存器里面写数值配置的时候，是按照8bit来写的，所以真正写的时候需要经过转换，公式为：
 * ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff)，其中的priority就是我们配置的真正的优先级。经过这个公式之后得到的是
 * 下面的这个宏：configKERNEL_INTERRUPT_PRIORITY
 * SysTick的优先级我们一般配置为最低，即0xf 。这样可以提高系统的实时响应能力，即其他的外部中断可以及时的得到响应。
 */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0xf
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/*===========================================可屏蔽的中断优先级配置====================================================*/
/*
 * 用于配置STM32的特殊寄存器basepri寄存器的值，用于屏蔽中断，当大于basepri值的优先级的中断将被全部屏蔽。basepri只有4bit有效，
 * 默认只为0，即全部中断都没有被屏蔽。configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY配置为：5，意思就是中断优先级大于5的中断都被屏蔽。
 * 当把配置好的优先级写到寄存器的时候，是按照8bit来写的，所以真正写的时候需要经过转换，公式为：
 * ((priority << (8 - __NVIC_PRIO_BITS)) & 0xff)，其中的priority就是我们配置的真正的优先级。经过这个公式之后得到的是下面的这个宏：
 * configMAX_SYSCALL_INTERRUPT_PRIORITY
 *
 * 在FreeRTOS中，关中断是通过配置basepri寄存器来实现的，关掉的中断由配置的basepri的值决定，小于basepri值的
 * 中断FreeRTOS是关不掉的，这样做的好处是可以系统设计者可以人为的控制那些非常重要的中断不能被关闭，在紧要的关头必须被响应。
 * 而在UCOS中，关中断是通过控制PRIMASK来实现的，PRIMASK是一个单1的二进制位，写1则除能除了NMI和硬 fault的所有中断。当UCOS关闭
 * 中断之后，即使是你在系统中设计的非常紧急的中断来了都不能马上响应，这加大了中断延迟的时间，如果是性命攸关的场合，那后果估计挺严重。
 * 相比UCOS的关中断的设计，FreeRTOS的设计则显得人性化很多。
 *
 */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5
 
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
	
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
//#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	
//断言
#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
#define configASSERT(x) if((x)==0) vAssertCalled(__FILE__,__LINE__)	
	
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
//#define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */

