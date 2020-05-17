#include "bsp_swi2c.h"

void swi2cStart(Swi2c_T * _tSwi2c)                 /*软件I2C起始信号*/

{
    _tSwi2c->xfpI2coutput();/*设置sda为输出模式*/

    *(_tSwi2c->ulpSdaOut) = 1;/*将sda置高*/

    *(_tSwi2c->ulpScl) = 1;/*将scl置高*/

    delay_us(5);

    *(_tSwi2c->ulpSdaOut) = 0;/*将sda置低*/

    delay_us(5);
    
    *(_tSwi2c->ulpScl) = 0;/*将scl置低*/
	
	delay_us(3);

}

void swi2cStop(Swi2c_T * _tSwi2c)                  /*软件I2C终止信号*/

{

   

     _tSwi2c->xfpI2coutput();/*设置sda为输出模式*/

     *(_tSwi2c->ulpSdaOut) = 0;/*将sda置高*/

	delay_us(1);

    *(_tSwi2c->ulpScl) = 1;/*将scl置高*/

    delay_us(1);
    
    *(_tSwi2c->ulpSdaOut) = 1;/*将sda置高*/	

    

    //*(_tSwi2c->ulpSdaOut) = 0;/*将sda置低*/

   // delay_us(5);

  



}

uint8_t swi2cWaitAck(Swi2c_T * _tSwi2c, uint8_t ucState)  /*软件I2C的ack应答信号*/

{
    int cTimeout = 50;  //定义超时时间

    _tSwi2c->xfpI2cinput();

    *(_tSwi2c->ulpScl) = 1;/*将scl置高*/
    
    if(ucState)     /*ack应答使能*/
    {
        while (*(_tSwi2c->ulpSdaIn))
        {
            delay_us(10);

            if((cTimeout--) < 0)
            {
                return 1;/*获取ack失败*/
            }
        }
    }

     delay_us(5);
              
     *(_tSwi2c->ulpScl) = 0;/*将scl置高*/

     delay_us(5);

    return 0;/*获取ack成功*/
}


void swi2cAck(Swi2c_T * _tSwi2c, uint8_t ucState)      /*软件I2C--master的发送一个ack应答*/

{
    _tSwi2c->xfpI2coutput(); /*设置为输出模式*/

    if(ucState)
        *(_tSwi2c->ulpSdaOut) = 0;

    delay_us(3);
	*(_tSwi2c->ulpScl) = 1; /*scl置高*/
	
	delay_us(2);
    *(_tSwi2c->ulpScl) = 0; /*scl置高*/
    delay_us(1);
    *(_tSwi2c->ulpSdaOut) = 1;
    delay_us(3);

}


uint8_t swi2cReceiveByte(Swi2c_T * _tSwi2c)           /*软件I2C的读取一个字节*/

{
    uint8_t ucData = 0;

    _tSwi2c->xfpI2cinput(); /*设置为输入模式*/

    

    for (int i = 0; i < 8; i++)
    {
        /* code */
        *(_tSwi2c->ulpScl) = 1; /*scl置高*/

        delay_us(2);

        ucData |=  *(_tSwi2c->ulpSdaIn);

        ucData <<= (i < 7 ? 1 : 0);

        delay_us(3);
    
        *(_tSwi2c->ulpScl) = 0;/*scl置低*/

        delay_us(5);

//        *(_tSwi2c->ulpScl) = 1; /*scl置高*/
    }
	return ucData;
}

void swi2cSendByte(Swi2c_T * _tSwi2c, uint8_t _ucData)              /*软件I2C的发送一个字节*/

{
    _tSwi2c->xfpI2coutput(); /*设置为输出模式*/

    for(int i = 7; i >= 0; i--)
    {
        *(_tSwi2c->ulpSdaOut) = (0x01) &(_ucData >> i);

        delay_us(2);

        *(_tSwi2c->ulpScl) = 1; /*scl置高*/

        delay_us(5);

        *(_tSwi2c->ulpScl) = 0; /*scl置高*/

        delay_us(3);
    }
}




