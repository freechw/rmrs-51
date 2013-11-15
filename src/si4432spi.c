#include "si4432spi.h"
#include "si4432spiport.h"
#include <intrins.h>
#include <STC/STC12C5A60S2.H>

#define NOP; _nop_();

void SPI_Init()
{
    SPDAT = 0x00;
    SPSTAT = 0xc0;
    SPCTL = 0xD0;
}

void SPI_Write_Reg(unsigned char reg, unsigned char value)
{
    NSEL = 0;
    NOP;
    SPSTAT = 0xc0;
    SPDAT = (reg | 0x80);
    while(!(0x80 & SPSTAT));
    SPSTAT = 0xc0;
    SPDAT = value;
    while(!(0x80 & SPSTAT));
    NSEL = 1;
    NOP;
}

unsigned char SPI_Read_Reg(unsigned char reg)
{
    unsigned char value;
    NSEL = 0;
    NOP;
    SPSTAT = 0xc0;
    SPDAT = reg;
    while(!(0x80 & SPSTAT));
    SPSTAT = 0xc0;
    SPDAT = 0xaa;
    while(!(0x80 & SPSTAT));
    value = SPDAT;
    NSEL = 1;
    NOP;
    return value;
}

void SPI_Burst_Write(unsigned char addr, unsigned char buf[], unsigned char length)
{
    unsigned char i;
    NSEL = 0;
    NOP;
    SPSTAT = 0xc0;
    SPDAT = (addr | 0x80);
    while(!(0x80 & SPSTAT));
    for (i = 0; i < length; i++)
    {
        SPSTAT = 0xc0;
        SPDAT = buf[i];
        while(!(0x80 & SPSTAT));
    }
    NSEL = 1;
    NOP;
}

void SPI_Burst_Read(unsigned char addr, unsigned char buf[], unsigned char length)
{
    unsigned char i;
    NSEL = 0;
    NOP;
    SPSTAT = 0xc0;
    SPDAT = addr;
    while(!(0x80 & SPSTAT));
    for (i = 0; i < length; i++)
    {
        SPSTAT = 0xc0;
        SPDAT = 0xbb;
        while(!(0x80 & SPSTAT));
        buf[i] = SPDAT;
    }
    NSEL = 1;
    NOP;
}