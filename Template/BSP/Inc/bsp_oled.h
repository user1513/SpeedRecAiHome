#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    
 
 

//OLED模式设置
//0: 4线串行SPI模式  （模块的BS1，BS2均接GND）
//1: 并行8080模式 （模块的BS1，BS2均接VCC）
//2: 2线IIC模式
#define OLED_MODE 	2 
		    						  
//-----------------OLED端口定义----------------  
#if OLED_MODE ==1
#define OLED_CS 	PBout(7)
#define OLED_RST  	PGout(15)	
#define OLED_RS 	PDout(6)
#define OLED_WR 	PAout(4)		  
#define OLED_RD 	PDout(7)
#elif OLED_MODE == 0
//使用4线串行接口时使用 
#define OLED_SCLK 	PCout(6)
#define OLED_SDIN 	PCout(7)
		     
#define OLED_CMD  	0		//写命令
#define OLED_DATA 	1		//写数据

#else
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	    						  
//-----------------OLED IIC端口定义----------------  					   
 		    
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_7)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOA,GPIO_Pin_8)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#endif
//OLED控制用函数
void OLED_WR_Byte(unsigned dat,unsigned cmd);	    
void OLED_Display_On(void);
void OLED_Display_Off(void);

void OLED_Refresh_Gram(void);		   
							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size, u8 mode);	



/*------------------------------------------------------------*/
/*----------------        自定义          --------------------*/
/*------------------------------------------------------------*/
void OLED_Clear_static(u8 state, u8 start, u8 end);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_show_chinese(u8 x,u8 y,u8 no);//显示单个汉字
void OLED_show_chinese_length(u8 x,u8 y,u8 no, uint8_t length);//显示多个汉字
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char BMP[]);
void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void Picture(void);


void OLED_Part_Clear(uint8_t start, uint8_t end);
int OLED_Show_32_32(u8 x,u8 y,u8 chr);
int OLED_Show_32_char(u8 x,u8 y,const u8 *able); //显示自定义32*16图形

#endif  
	 



