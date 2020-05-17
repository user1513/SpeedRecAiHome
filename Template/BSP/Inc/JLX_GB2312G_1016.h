#ifndef JLX_GB2312G_1016_H
#define JLX_GB2312G_1016_H

#include "sys.h"

#define JLX_CS_GB2312G 		PEout(0)
#define JLX_RESET_GB2312G 	PEout(1)
#define JLX_RS_GB2312G 		PEout(2)
#define JLX_SDA_GB2312G 	PEout(3)
#define JLX_SCLK_GB2312G 	PEout(4)

void initial_lcd(void);//LCD 模块初始化

void clear_screen(void);//全屏清屏

void lx_Gb2312g_Write_7X8_Ascll(char uAscll, uint8_t page, uint8_t column, uint8_t Convet);

void lx_Gb2312g_Write_5X7_Ascll(char uAscll, uint8_t page, uint8_t column, uint8_t Convet);

void lx_Gb2312g_Write_8X16_Ascll(char uAscll, uint8_t page, uint8_t column, uint8_t uIsBold, uint8_t Convet);

void lx_Gb2312g_Write_15X16_Chinese(char *uAscll, uint8_t page, uint8_t column, uint8_t Convet);

void lx_Gb2312g_Str(char *uAscll,uint8_t page, uint8_t column, uint8_t Convet);

void lx_Gb2312g_Write_5X7_Ascll_Str(char* pAscll, uint8_t page, uint8_t column, uint8_t Convet);
#endif

