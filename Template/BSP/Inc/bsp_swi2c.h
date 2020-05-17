#ifndef _BSP_SWI2C_H
#define _BSP_SWI2C_H

#include "bsp.h"

typedef void (*I2C_FUNC)(void);

typedef struct tagSwi2c_T
{
    /* data */
    unsigned long* ulpScl;              /*放入指定scl地址*/

    unsigned long* ulpSdaOut;           /*放入指定sda地址*/

    unsigned long* ulpSdaIn;            /*放入指定sda地址*/

    I2C_FUNC    xfpI2cinput;            /*自定义函数型指针,设置指定sda为输入模式*/

    I2C_FUNC    xfpI2coutput;           /*自定义函数型指针,设置指定sda为输出模式*/

}Swi2c_T;

void swi2cStart(Swi2c_T * _tSwi2c);                 /*软件I2C起始信号*/

void swi2cStop(Swi2c_T * _tSwi2c);                  /*软件I2C终止信号*/

uint8_t swi2cReceiveByte(Swi2c_T * _tSwi2c);        /*软件I2C的读取一个字节*/

void swi2cSendByte(Swi2c_T * _tSwi2c, uint8_t _ucData);  /*软件I2C的发送一个字节*/

uint8_t swi2cWaitAck(Swi2c_T * _tSwi2c, uint8_t ucState);/*软件I2C的等待slave的 ack应答信号*/

void swi2cAck(Swi2c_T * _tSwi2c, uint8_t ucState);      /*软件I2C--master的发送一个ack应答*/

#endif




