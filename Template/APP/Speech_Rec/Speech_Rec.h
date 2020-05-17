#ifndef _SPPECH_REC_H
#define _SPPECH_REC_H

#include "stdint.h"
/*语音识别处理函数*/
void Speech_Handle(uint8_t _ucMode);

void SpeechRecUartPack(void);
/*用于将一段数据打包成合格的数据包结构*/
uint32_t UartDataPacking(char* str, uint8_t type, uint32_t length, uint8_t Check);
#endif
