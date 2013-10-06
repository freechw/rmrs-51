#include "store.h"
#include "interseral.h"
#include <STC/STC12C5A60S2.H>
#include <intrins.h>


#define NOP _nop_();

sbit SDA = P1^3;
sbit SCL = P1^4;

#define SI4432ID_ADDR 0x0000
#define SYNCWORD_ADDR 0x0002
#define COMMAND_BUF_ADDR 0x0040
#define TOTAL_NUM_ADDR 0x0041
#define READ_NUM_ADDR 0x0042
#define TRANS_NUM_ADDR 0x0043
#define PAGE_BASE_ADDR 0x0080

void Delay11ms()		//@11.0592MHz
{
    unsigned char i, j;

    _nop_();
    i = 120;
    j = 82;
    do
    {
        while (--j);
    } while (--i);
}

void StoreInit()
{
    SCL = 1;
    SDA = 1;
}

void Start()
{
    SDA = 1;
    NOP;
    SCL = 1;
    NOP;
    SDA = 0;
    NOP;
    SCL = 0;
    NOP;
}

void Stop()
{
    SDA = 0;
    NOP;
    SCL = 1;
    NOP;
    SDA = 1;
    NOP;
}

void Write(unsigned char byte)
{
    char i = 8;
    while (i-- > 0)
    {
        SCL = 0;
        NOP;
        SDA = (bit)(byte & 0x80);
        NOP;
        SCL = 1;
        NOP;
        byte = byte << 1;
    }
    SCL = 0;
    NOP;
    SDA = 1;
    NOP;
}

unsigned char Read()
{
    unsigned char byte = 0;
    char i = 8;
    SCL = 0;
    NOP;
    SDA = 1;//let the sda pin input
    NOP;
    while ( i-- >0)
    {
        SCL = 1; 
        NOP;
        byte = (byte << 1) | SDA;   
        SCL = 0;
        NOP;
    }
    return byte;
}

char WaitACK()
{
    unsigned char i = 0;
    char ack = -1;

    SCL = 0;
    NOP;
    SCL = 1;
    NOP;
    while((SDA == 1) && (i < 255))
    {
        i++;
        NOP;
    }
    if( i < 255)
    {
        ack = 0;
    }
    SCL = 0;
    NOP;

    return ack;
}

void SendACK()
{
    SDA = 0;
    NOP;
    SCL = 0;
    NOP;
    SCL = 1;
    NOP;// wait 24C256 get the ack signal;
    SCL = 0;
    NOP;
}

void SendNoACK()
{
    SDA = 1;
    NOP;
    SCL = 0;
    NOP;
    SCL = 1;
    NOP;// wati 24C256 get the noack signal;
    SCL = 0;
    NOP;
}

void _IN_WritePage(unsigned char buf[], unsigned char length, unsigned int addr)
{
    unsigned char i = 0;

    Start();
    Write(0xa0);
    WaitACK();
    Write(addr >> 8);
    WaitACK();
    Write(addr & 0xff);
    WaitACK();

    for ( i = 0; i < length; i++)
    {
        Write(buf[i]);
        WaitACK();
    }
    Stop();
    Delay11ms();

}

void _IN_ReadPage(unsigned char buf[], unsigned char length, unsigned int addr)
{
    unsigned char i = 0;

    ET1 = 0;//close timer 1 interrupt

    Start();
    Write(0xa0);
    WaitACK();
    Write(addr >> 8);
    WaitACK();
    Write(addr & 0xff);
    WaitACK();
    Start();
    Write(0xa1);
    WaitACK();

    for ( i = 0; i < length - 1; i++)
    {
        buf[i] = Read();
        SendACK();
    }
    buf[i] = Read();
    SendNoACK();
    Stop();

    ET1 = 1;
}

void storeTest(unsigned char ch, unsigned int addr) 
{
    unsigned char tmp;
    _IN_WritePage(&ch, 1, addr);
    _IN_ReadPage(&tmp, 1, addr);

    if( tmp != ch)
    {
        InterSendString("I2C: TEST: test fail!\r\n");
        InterSendByte(tmp);
    }
    else
    {
        InterSendString("I2C: TEST: test OK!\r\n");
    }
}

void storeWriteTotalNum(unsigned char num) reentrant
{
    _IN_WritePage(&num, 1, TOTAL_NUM_ADDR);
}
unsigned char storeReadTotalNum() reentrant
{
    unsigned char num;
    _IN_ReadPage(&num, 1, TOTAL_NUM_ADDR);
    return num;
}

void storeWriteReadNum(unsigned char num) reentrant
{
    unsigned char tmpET1;
    tmpET1 = ET1;//cache ET1 status
    ET1 = 0;
    _IN_WritePage(&num, 1, READ_NUM_ADDR);
    ET1 = tmpET1;//restore ET1 status
}
unsigned char storeReadReadNum() reentrant
{
    unsigned char num;
    unsigned char tmpET1;
    tmpET1 = ET1;//cache ET1 status
    ET1 = 0;
    _IN_ReadPage(&num, 1, READ_NUM_ADDR);
    ET1 = tmpET1;
    return num;
} 

void storeWriteTransNum(unsigned char num) reentrant
{
    unsigned char tmpET1;
    tmpET1 = ET1;//cache ET1 status
    ET1 = 0;
    _IN_WritePage(&num, 1, TRANS_NUM_ADDR);
    ET1 = tmpET1;
}
unsigned char storeReadTransNum() reentrant
{
    unsigned char num;
    unsigned char tmpET1;
    tmpET1 = ET1;//cache ET1 status
    ET1 = 0;
    _IN_ReadPage(&num, 1, TRANS_NUM_ADDR);
    ET1 =tmpET1;
    return num;
}

void storeWriteCommandBuf(unsigned char buf[], unsigned char length) reentrant
{
    unsigned char tmpET1;
    tmpET1 = ET1;//cache ET1 status
    ET1 = 0;
    _IN_WritePage(buf, length, COMMAND_BUF_ADDR);
    ET1 = tmpET1;
}
void storeReadCommandBuf(unsigned char buf[], unsigned char length) reentrant
{
    unsigned char tmpET1;
    tmpET1 = ET1;//cache ET1 status
    ET1 = 0;
    _IN_ReadPage(buf, length, COMMAND_BUF_ADDR);
    ET1 = tmpET1;
}

void storeWritePage(unsigned char pageNum, unsigned char buf[], unsigned char length) reentrant
{
    unsigned char tmpET1;
    tmpET1 = ET1;//cache ET1 status
    ET1 = 0;
    _IN_WritePage(buf, length, (PAGE_BASE_ADDR + pageNum * 64));
    ET1 = tmpET1;
}

void storeReadPage(unsigned char pageNum, unsigned char buf[], unsigned char length) reentrant
{
    unsigned char tmpET1;
    tmpET1 = ET1;//cache ET1 status
    ET1 = 0;
    _IN_ReadPage(buf, length, (PAGE_BASE_ADDR + pageNum * 64));
    ET1 = tmpET1;
}

void storeWriteSi4432Id(unsigned int si4432Id)
{
    _IN_WritePage((unsigned char *)&si4432Id, 2, SI4432ID_ADDR);
}

unsigned int storeReadSi4432Id()
{
    unsigned int si4432Id;
    _IN_ReadPage((unsigned char *)&si4432Id, 2, SI4432ID_ADDR);
    return si4432Id;
}

void storeWriteSyncword(unsigned int syncword)
{
    _IN_WritePage((unsigned char *)(&syncword), 2, SYNCWORD_ADDR);
}

unsigned int storeReadSyncword()
{
    unsigned int syncword;
    _IN_ReadPage((unsigned char *)(&syncword), 2, SYNCWORD_ADDR);
    return syncword;
}



