#include "JLX_GB2312z.h"
#include "delay.h"
#include "bsp_spi.h"

static uint8_t Get_15X16_GB2312_Adder(char *uGb2312, uint32_t *uWordAdder);
static uint8_t Get_5X7_Ascll_Adder(uint8_t uAscll, uint32_t *uWordAdder);
static uint8_t Get_7X8_Ascll_Adder(uint8_t uAscll, uint32_t *uWordAdder);
static uint8_t Get_8X16_Ascll_Adder(uint8_t uAscll, uint32_t *uWordAdder);
static uint8_t Get_8X16_Ascll_Bold_Adder(uint8_t uAscll, uint32_t *uWordAdder);

uint8_t Get_JLX_15X16_GB2312(char *uGb2312, uint8_t* sAscll, uint8_t Convet)
{
    uint32_t uWordAdder, uIsok;
	if(0 == (uIsok = Get_15X16_GB2312_Adder(uGb2312, &uWordAdder)))
	{
        ROM_CS = 0;
    	sAscll[32] = SPI1_ReadWriteByte(0x03);
    	sAscll[33] = SPI1_ReadWriteByte((uWordAdder&0x00FF0000)>>16);
    	sAscll[34] = SPI1_ReadWriteByte((uWordAdder&0x0000FF00)>>8);
    	sAscll[35] = SPI1_ReadWriteByte(uWordAdder&0x000000FF);
    	for(int i = 0; i < 32; i++)
    	{
    		if(0 == Convet)
				sAscll[i] = SPI1_ReadWriteByte(0xff);
			else
				sAscll[i] = ~SPI1_ReadWriteByte(0xff);
    	}
    	delay_us(15);
    	ROM_CS = 1;
    }
	return uIsok;
}

uint8_t Get_JLX_5X7_Ascll(uint8_t uAscll, uint8_t* sAscll, uint8_t Convet)
{
    uint32_t uWordAdder, uIsok;
	if(0 == (uIsok = Get_5X7_Ascll_Adder(uAscll, &uWordAdder)))
	{
        ROM_CS = 0;
    	sAscll[8] = SPI1_ReadWriteByte(0x03);
    	sAscll[9] = SPI1_ReadWriteByte((uWordAdder&0x00FF0000)>>16);
    	sAscll[10] = SPI1_ReadWriteByte((uWordAdder&0x0000FF00)>>8);
    	sAscll[11] = SPI1_ReadWriteByte(uWordAdder&0x000000FF);
    	for(int i = 0; i < 8; i++)
    	{
    		if(0 == Convet)
				sAscll[i] = SPI1_ReadWriteByte(0xff);
			else
				sAscll[i] = ~SPI1_ReadWriteByte(0xff);
    	}
    	delay_us(15);
    	ROM_CS = 1;
    }
	return uIsok;
}

uint8_t Get_JLX_7X8_Ascll(uint8_t uAscll, uint8_t* sAscll, uint8_t Convet)
{
    uint32_t uWordAdder, uIsok;
	if(0 == (uIsok = Get_7X8_Ascll_Adder(uAscll, &uWordAdder)))
	{
        ROM_CS = 0;
    	sAscll[8] = SPI1_ReadWriteByte(0x03);
    	sAscll[9] = SPI1_ReadWriteByte((uWordAdder&0x00FF0000)>>16);
    	sAscll[10] = SPI1_ReadWriteByte((uWordAdder&0x0000FF00)>>8);
    	sAscll[11] = SPI1_ReadWriteByte(uWordAdder&0x000000FF);
    	for(int i = 0; i < 8; i++)
    	{
    		if(0 == Convet)
				sAscll[i] = SPI1_ReadWriteByte(0xff);
			else
				sAscll[i] = ~SPI1_ReadWriteByte(0xff);
    	}
    	delay_us(50);
    	ROM_CS = 1;
    }
	return uIsok;
}

uint8_t Get_JLX_8X16_Ascll(uint8_t uAscll, uint8_t* sAscll, uint8_t Convet)
{
    uint32_t uWordAdder, uIsok;
	if(0 == (uIsok = Get_8X16_Ascll_Adder(uAscll, &uWordAdder)))
	{
        ROM_CS = 0;
    	sAscll[16] = SPI1_ReadWriteByte(0x03);
    	sAscll[17] = SPI1_ReadWriteByte((uWordAdder&0x00FF0000)>>16);
    	sAscll[18] = SPI1_ReadWriteByte((uWordAdder&0x0000FF00)>>8);
    	sAscll[19] = SPI1_ReadWriteByte(uWordAdder&0x000000FF);
    	for(int i = 0; i < 16; i++)
    	{
    		if(0 == Convet)
				sAscll[i] = SPI1_ReadWriteByte(0xff);
			else
				sAscll[i] = ~SPI1_ReadWriteByte(0xff);
    	}
    	delay_us(50);
    	ROM_CS = 1;
    }
	return uIsok;
}

uint8_t Get_JLX_8X16_Bold_Ascll(uint8_t uAscll, uint8_t* sAscll, uint8_t Convet)
{
    uint32_t uWordAdder, uIsok;
	if(0 == (uIsok = Get_8X16_Ascll_Bold_Adder(uAscll, &uWordAdder)))
	{
        ROM_CS = 0;
    	sAscll[16] = SPI1_ReadWriteByte(0x03);
    	sAscll[17] = SPI1_ReadWriteByte((uWordAdder&0x00FF0000)>>16);
    	sAscll[18] = SPI1_ReadWriteByte((uWordAdder&0x0000FF00)>>8);
    	sAscll[19] = SPI1_ReadWriteByte(uWordAdder&0x000000FF);
    	for(int i = 0; i < 16; i++)
    	{
			if(0 == Convet)
				sAscll[i] = SPI1_ReadWriteByte(0xff);
			else
				sAscll[i] = ~SPI1_ReadWriteByte(0xff);
    	}
    	delay_us(50);
    	ROM_CS = 1;
    }
	return uIsok;
}

static uint8_t Get_15X16_GB2312_Adder(char *uGb2312, uint32_t *uWordAdder)
{
    uint8_t uIsok = 0;
    uint8_t uMsb = *uGb2312, uLsb = *(uGb2312 + 1);
    uint32_t uBaseAdder = FONT_15X16_GB2312_BASEADDER;
    if(0xA9 == uMsb && 0xA1 <= uLsb)
        *uWordAdder = (282 + (uLsb - 0xA1)) * 32 + uBaseAdder;
    else if(0xA1 <= uMsb && 0xA3 >= uMsb && 0xA1 <= uLsb)
        *uWordAdder = ((uMsb - 0xA1) * 94 + (uLsb - 0xA1)) * 32 + uBaseAdder;
    else if(0xB0 <= uMsb && 0xF7 >= uMsb && 0xA1 <= uLsb)
        *uWordAdder = ((uMsb - 0xB0) * 94 + (uLsb - 0xA1) + 846) * 32 + uBaseAdder;
    else
        uIsok = 0xff;
    return uIsok;
}


static uint8_t Get_5X7_Ascll_Adder(uint8_t uAscll, uint32_t *uWordAdder)
{
    uint8_t uIsok = 0;
    uint32_t uBaseAdder = FONT_5X7_ASCLL_BASEADDER;

    if((127 >= uAscll) && (uAscll >= ' '))
        *uWordAdder = uBaseAdder + (uAscll - ' ') * 8;
    else
        uIsok = 0xff;
    return uIsok;
}

static uint8_t Get_7X8_Ascll_Adder(uint8_t uAscll, uint32_t *uWordAdder)
{
    uint8_t uIsok = 0;
    uint32_t uBaseAdder = FONT_7X8_ASCLL_BASEADDER;

    if((127 >= uAscll) && (uAscll >= ' '))
        *uWordAdder = uBaseAdder + (uAscll - ' ') * 8;
    else
        uIsok = 0xff;
    return uIsok;
}

static uint8_t Get_8X16_Ascll_Adder(uint8_t uAscll, uint32_t *uWordAdder)
{
    uint8_t uIsok = 0;
    uint32_t uBaseAdder = FONT_8X16_ASCLL_BASEADDER;

    if((127 >= uAscll) && (uAscll >= ' '))
        *uWordAdder = uBaseAdder + (uAscll - ' ') * 16;
    else
        uIsok = 0xff;
    return uIsok;
}

static uint8_t Get_8X16_Ascll_Bold_Adder(uint8_t uAscll, uint32_t *uWordAdder)
{
    uint8_t uIsok = 0;
    uint32_t uBaseAdder = FONT_8X16_ASCLLBOLD_BASEADDER;

    if((127 >= uAscll) && (uAscll >= ' '))
        *uWordAdder = uBaseAdder + (uAscll - ' ') * 16;
    else
        uIsok = 0xff;
    return uIsok;
}




//		GPIO_ResetBits(GPIOB,GPIO_Pin_12);
//		delay_us(50);
//		SPI2_ReadWriteByte(0x0b);
//		SPI2_ReadWriteByte(0x00);
//		SPI2_ReadWriteByte(0x66);
//		SPI2_ReadWriteByte(0xC0+num);
//		SPI2_ReadWriteByte(0x01);
//		for(int i = 0; i < 7; i++)
//		{
//			able[i] = SPI2_ReadWriteByte(0x00);
//		}
//		delay_us(50);
//		GPIO_SetBits(GPIOB,GPIO_Pin_12);



















































