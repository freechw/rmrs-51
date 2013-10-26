#include "ch438uart1.h"
#include "ch438direct_simulate.h"
#include <STC/STC12C5A60S2.H>

#define CH438UART1_BPS 2400
#define Fpclk 1843200

void CloseOtherUart();

void InitCh438Uart1()
{
    unsigned short div;
    unsigned char DLL, DLM;

    CloseOtherUart();

    div = (Fpclk >> 4) / CH438UART1_BPS;
    DLM = div >> 8;
    DLL = div & 0xff;

    simWrite(IER1, 0x80);//reset uart 1

    simWrite(LCR1, 0x80);
    simWrite(DLL1, DLL);
    simWrite(DLM1, DLM);

    simWrite(FCR1, 0xc7);//FCR1 = 0xc7;//Enable FIFO, Set 112bit FITO and Clear FITO
    simWrite(LCR1, 0x1b);//LCR1 = 0x1b;//even parity 8bit data 1bit stop
    simWrite(IER1, 0x01);//IER1 = 0x01;//only open recv interrupt
    simWrite(MCR1, 0x08);//MCR1 = 0x0f;//Enable Interrput Pin
    simWrite(FCR1, 0xc7);//FCR1 = 0xc7;//112bit FIFO and clear FIFO
}

void CloseOtherUart()
{
    simWrite(IER0, 0x40);
    simWrite(IER2, 0x40);
    simWrite(IER3, 0x40);
    simWrite(IER4, 0x40);
    simWrite(IER5, 0x40);
    simWrite(IER6, 0x40);
    simWrite(IER7, 0x40);

    simWrite(MCR0, 0x00);
    simWrite(MCR2, 0x00);
    simWrite(MCR3, 0x00);
    simWrite(MCR4, 0x00);
    simWrite(MCR5, 0x00);
    simWrite(MCR6, 0x00);
    simWrite(MCR7, 0x00);
}

void Ch438Uart1SendByte(unsigned char byte)
{
    while(0x20 != (0x20 & simRead(LSR1)));
    simWrite(THR1, byte);//THR1 = byte;
}

void Ch438Uart1SendBuf(unsigned char buf[], unsigned char length)
{
    unsigned char i = 0;
    while(0x40 != (0x40 & simRead(LSR1)));
    simWrite(FCR1, (0x40 | simRead(FCR1)));//clear TX FIFO
    for (i = 0; i < length; i++)
    {
        simWrite(THR1, buf[i]);
    }
}
void Ch438Uart1Send(unsigned char * buf, unsigned char length)
{
    unsigned char buf_idx = 0;
    while(buf_idx < length)
    {
        Ch438Uart1SendByte(buf[buf_idx++]);
    }
}

unsigned char Ch438Uart1Read(unsigned char * buf)
{
    unsigned char buf_idx = 0;
    while(0x01 == (0x01 & simRead(LSR1)))
    {
        buf[buf_idx++] = simRead(RBR1);
    }
    simWrite(FCR1, (0x20 | simRead(FCR1)));//clear RX FIFO
    return buf_idx;
}

void Ch438Uart1SendString(unsigned char * string)
{
    while('\0' != *string)
    {
        Ch438Uart1SendByte(*string++);
    }
}

//void InitCh438Uart1()
//{
//    unsigned short div;
//    unsigned char DLL, DLM;
//
//    CloseOtherUart();
//
//    div = (Fpclk >> 4) / CH438UART1_BPS;
//    DLM = div >> 8;
//    DLL = div & 0xff;
//
//    IER1 = 0x80;//reset uart 1
//
//    LCR1 = 0x80;
//    DLL1 = DLL;
//    DLM1 = DLM;
//
//    FCR1 = 0xc7;//Enable FIFO, Set 112bit FITO and Clear FITO
//    LCR1 = 0x1b;//even parity 8bit data 1bit stop
//    IER1 = 0x01;//only open recv interrupt
//    MCR1 = 0x0f;//Enable Interrput Pin
//    FCR1 = 0xc7;//112bit FIFO and clear FIFO
//}
//
//void CloseOtherUart()
//{
//    IER0 = 0x40;
//    IER2 = 0x40;
//    IER3 = 0x40;
//    IER4 = 0x40;
//    IER5 = 0x40;
//    IER6 = 0x40;
//    IER7 = 0x40;
//
//    MCR0 = 0x00;
//    MCR2 = 0x00;
//    MCR3 = 0x00;
//    MCR4 = 0x00;
//    MCR5 = 0x00;
//    MCR6 = 0x00;
//    MCR7 = 0x00;
//}
//
//void Ch438Uart1SendByte(unsigned char byte)
//{
//    while(0x20 != (0x20 & LSR1));
//    THR1 = byte;
//}
//void Ch438Uart1Send(unsigned char * buf, unsigned char length)
//{
//    unsigned char buf_idx = 0;
//    while(buf_idx < length)
//    {
//        Ch438Uart1SendByte(buf[buf_idx++]);
//    }
//}
//
//unsigned char Ch438Uart1Read(unsigned char * buf)
//{
//    unsigned char buf_idx = 0;
//    while(0x01 == (0x01 & LSR1))
//    {
//        buf[buf_idx++] = RBR1;
//    }
//    
//    return buf_idx;
//}
//
//void Ch438Uart1SendString(unsigned char * string)
//{
//    while('\0' != *string)
//    {
//        Ch438Uart1SendByte(*string++);
//    }
//}
