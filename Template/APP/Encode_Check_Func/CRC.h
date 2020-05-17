#ifndef __CRC_H__
#define __CRC_H__
#include <stdint.h>

typedef unsigned char        BOOL;
#define FALSE                0
#define TRUE                 1

typedef struct
{
	uint8_t poly;//多项式
	uint8_t InitValue;//初始值
	uint8_t _xor;//结果异或值
	BOOL InputReverse;
	BOOL OutputReverse;
}CRC_8;

typedef struct
{
	uint16_t poly;//多项式
	uint16_t InitValue;//初始值
	uint16_t _xor;//结果异或值
	BOOL InputReverse;
	BOOL OutputReverse;
}CRC_16;

typedef struct
{
	uint32_t poly;//多项式
	uint32_t InitValue;//初始值
	uint32_t _xor;//结果异或值
	BOOL InputReverse;
	BOOL OutputReverse;
}CRC_32;

/*已经存在的crc8_crc16_crc32不存在的需要自己定义*/
extern const CRC_8 crc_8;
extern const CRC_8 crc_8_ITU;
extern const CRC_8 crc_8_ROHC;
extern const CRC_8 crc_8_MAXIM;

extern const CRC_16 crc_16_IBM;
extern const CRC_16 crc_16_MAXIM;
extern const CRC_16 crc_16_USB;
extern const CRC_16 crc_16_MODBUS;
extern const CRC_16 crc_16_CCITT;
extern const CRC_16 crc_16_CCITT_FALSE;
extern const CRC_16 crc_16_X5;
extern const CRC_16 crc_16_XMODEM;
extern const CRC_16 crc_16_DNP;

extern const CRC_32 crc_32;
extern const CRC_32 crc_32_MPEG2;

uint8_t crc8(uint8_t* addr, int num, CRC_8* type);
uint16_t crc16(uint8_t* addr, int num, CRC_16* type);
uint32_t crc32(uint8_t* addr, int num, CRC_32* type);

#endif



