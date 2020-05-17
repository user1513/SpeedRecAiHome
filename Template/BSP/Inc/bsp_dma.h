#ifndef _BSP_DMA_H
#define _BSP_DMA_H
#include "bsp.h"

void bspDmaUsartInit(void);

/*USART数据开始发送*/
void usartSendStart(uint8_t* buf,uint16_t buf_count);

/*USART数据停止发送*/
void usartSendStop(void);

/*USART数据数据接收开始*/
void usartReadStart(uint8_t* buf,uint16_t buf_count);

/*USART数据接收停止*/
void usartReadStop(void);

/*设置dma模式*/
void SetDmaStatus(uint8_t _ucMode);

/*获取dma模式*/
uint8_t GetDmaStatus(void);

/*切换当前dma读地址*/
uint8_t* swich_current_read_point(void);

/*获取当前串口读取完成的指针*/
uint8_t* read_current_finaliy_point(void);

#endif

