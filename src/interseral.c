#include "interseral.h"
#include <STC/STC12C5A60S2.H>

#define INTER_STR_BUF_LENGTH 16
idata unsigned char strBuf[INTER_STR_BUF_LENGTH * 2];

data unsigned char interSeralRecv_Flag = 0;

void InterSeralInit()//115200 bps @11.0592Mhz
{
    PCON &= 0x7F;       //波特率不倍速
    SCON = 0x50;        //8位数据,可变波特率
    AUXR |= 0x04;       //独立波特率发生器时钟为Fosc,即1T
    BRT = 0xFD;         //设定独立波特率发生器重装值
    AUXR |= 0x01;       //串口1选择独立波特率发生器为波特率发生器
    AUXR |= 0x10;       //启动独立波特率发生器

    ES = 0;
    TI = 1;
}

void InterSendByte(unsigned char byte)
{
    while (0 == TI);
    TI = 0;
    SBUF = byte;
}

unsigned char InterRecvByte()
{
    unsigned char byte;
    if(1 == RI)
    {
        byte = SBUF;
        interSeralRecv_Flag = 1;
        RI = 0;
    }

    return byte;
}

void InterSend(unsigned char buf[], unsigned char length)
{
    unsigned char buf_idx = 0;

    while(length--)
    {
        while(!TI);
        InterSendByte(buf[buf_idx++]);
    }
}

void InterSendString(unsigned char * buf)
{
    unsigned char buf_idx = 0;

    while('\0' != buf[buf_idx])
    {
        InterSendByte(buf[buf_idx++]);
    }

}

void InterHexString(unsigned char buf[], unsigned char length)
{
    unsigned char times = length / INTER_STR_BUF_LENGTH;//calc the number of complete block(length is INTER_STR_BUF_LENGTH)
    unsigned char i;
    unsigned char j;
    //send each complete block
    for (i = 0; i < times; i++)
    {
        for (j = 0; j < INTER_STR_BUF_LENGTH; j++)
        {
            if (9 >= (buf[j + (i * INTER_STR_BUF_LENGTH) ] / 16) && 0<= (buf[j + (i * INTER_STR_BUF_LENGTH)] / 16))
            {
                strBuf[2 * j] = (buf[j + (i * INTER_STR_BUF_LENGTH)] / 16) + 0x30;
            }
            else
            {
                strBuf[2 * j] = (buf[j + (i * INTER_STR_BUF_LENGTH)] / 16) + 0x37;
            }
            if (9 >= (buf[j + (i * INTER_STR_BUF_LENGTH) ] % 16) && 0<= (buf[j + (i * INTER_STR_BUF_LENGTH)] % 16))
            {
                strBuf[(2 * j) + 1] = (buf[j + (i * INTER_STR_BUF_LENGTH)] % 16) + 0x30;
            }
            else
            {
                strBuf[(2 * j) + 1] = (buf[j + (i * INTER_STR_BUF_LENGTH)] % 16) + 0x37;
            }
        }
        //send this block  to InterSeral
        InterSend(strBuf, 2 * INTER_STR_BUF_LENGTH);
    }
    //send the last bytes of buf(length is less than INTER_STR_BUF_LENGTH)
    for (j = 0; j < (length - (times * INTER_STR_BUF_LENGTH)); j++)
    {
        if (9 >= (buf[j + (times * INTER_STR_BUF_LENGTH) ] / 16) && 0<= (buf[j + (times * INTER_STR_BUF_LENGTH)] / 16))
        {
            strBuf[2 * j] = (buf[j + (times * INTER_STR_BUF_LENGTH)] / 16) + 0x30;
        }
        else
        {
            strBuf[2 * j] = (buf[j + (times * INTER_STR_BUF_LENGTH)] / 16) + 55;
        }
        if (9 >= (buf[j + (times * INTER_STR_BUF_LENGTH) ] % 16) && 0<= (buf[j + (times * INTER_STR_BUF_LENGTH)] % 16))
        {
            strBuf[(2 * j) + 1] = (buf[j + (times * INTER_STR_BUF_LENGTH)] % 16) + 0x30;
        }
        else
        {
            strBuf[(2 * j) + 1] = (buf[j + (times * INTER_STR_BUF_LENGTH)] % 16) + 55;
        }
    }
    //send last byte to InterSeral
    InterSend(strBuf, 2 * (length - (times * INTER_STR_BUF_LENGTH)));
}

