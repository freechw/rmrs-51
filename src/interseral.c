#include "interseral.h"
#include <STC/STC12C5A60S2.H>

bit interSeralRecv_Flag = 0;

idata unsigned char strBuf[100];

void InterSeralInit()//115200 bps @11.0592Mhz
{
    PCON &= 0x7F;       //�����ʲ�����
    SCON = 0x50;        //8λ����,�ɱ䲨����
    AUXR |= 0x04;       //���������ʷ�����ʱ��ΪFosc,��1T
    BRT = 0xFD;         //�趨���������ʷ�������װֵ
    AUXR |= 0x01;       //����1ѡ����������ʷ�����Ϊ�����ʷ�����
    AUXR |= 0x10;       //�������������ʷ�����

    ES = 0;
    TI = 1;
}

//void InterSeralInit(void)        //115200bps@12.000MHz
//{
//    PCON |= 0x80;        //ʹ�ܲ����ʱ���λSMOD
//    SCON = 0x50;        //8λ����,�ɱ䲨����
//    AUXR |= 0x04;        //���������ʷ�����ʱ��ΪFosc,��1T
//    BRT = 0xF9;        //�趨���������ʷ�������װֵ
//    AUXR |= 0x01;        //����1ѡ����������ʷ�����Ϊ�����ʷ�����
//    AUXR |= 0x10;        //�������������ʷ�����
//
//    ES = 0;
//    TI = 1;
//}

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
    unsigned char i;
    for (i = 0; (i < length) && (i < 100); i++)
    {
        if ((buf[i]/16) <= 9 && (buf[i]/16) >=0)
        {
            strBuf[2 * i] = (buf[i]/16) + 0x30;
        }
        else
        {
            strBuf[2 * i] = (buf[i]/16) + 55;
        }
        if ((buf[i]%16) <= 9 && (buf[i]%16) >=0)
        {
            strBuf[(2 * i) + 1] = (buf[i]%16) + 0x30;
        }
        else
        {
            strBuf[(2 * i) + 1] = (buf[i]%16) + 55;
        }
    }

    InterSend(strBuf, 2 * length);
}
