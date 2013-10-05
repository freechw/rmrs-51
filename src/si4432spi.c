#include "si4432spi.h"
#include "si4432spiport.h"
#include <intrins.h>
#include <STC/STC12C5A60S2.H>

#define NOP; _nop_();

void SPI_Write_Reg(unsigned char reg, unsigned char value)
{
    unsigned char i;
    unsigned char Temp_byte;

    NSEL = 0;
    NOP;
    Temp_byte = (reg|0x80);

    for(i = 0; i < 8; i++)
    {
        SDI = (Temp_byte&0x80);
        NOP;
        SCLK = 1;
        NOP;
        SCLK = 0;
        NOP;
        Temp_byte <<= 0x01;
    }

    Temp_byte = value;
    for(i = 0; i < 8; i++)
    {
        SDI = (Temp_byte&0x80);
        NOP;
        SCLK = 1;
        NOP;
        SCLK = 0;
        NOP;
        Temp_byte <<= 0x01;
    }

    NSEL = 1;
    NOP;
}

unsigned char SPI_Read_Reg(unsigned char reg)
{
    unsigned char i;
    unsigned char Temp_byte;
    unsigned char Result;

    NSEL = 0;
    NOP;
    Temp_byte = reg;

    for(i = 0; i < 8; i++)
    {
        SDI = (Temp_byte&0x80);
        NOP;
        SCLK = 1;
        NOP;
        SCLK = 0;
        NOP;
        Temp_byte <<= 0x01;
    }

    Result = 0;
    SDO = 1;//let the sdo pin input
    for(i = 0; i < 8; i++)
    {
        Result = Result<<1;
        SCLK = 1;
        NOP;

        if(SDO)
        {
            Result |= 1;
        }

        SCLK = 0;
        NOP;
    }

    NSEL = 1;

    return(Result);
}

void SPI_Burst_Write(unsigned char addr, unsigned char buf[], unsigned char length)
{
    unsigned char Temp_byte;
    unsigned char i;
    unsigned char j;

    Temp_byte = (addr|0x80);

    NSEL = 0;
    NOP;

    for(i = 0; i < 8; i++)
    {
        SDI = (Temp_byte&0x80);
        NOP;
        SCLK = 1;
        NOP;
        SCLK = 0;
        NOP;
        Temp_byte <<= 0x01;
    }

    for (i = 0; i < length; i++)
    {
        Temp_byte = buf[i];
        for(j = 0; j < 8; j++)
        {
            SDI = (Temp_byte&0x80);
            NOP;
            SCLK = 1;
            NOP;
            SCLK = 0;
            NOP;
            Temp_byte <<= 0x01;
        }
    }

    NSEL = 1;
    NOP;
}
void SPI_Burst_Read(unsigned char addr, unsigned char buf[], unsigned char length)
{
    unsigned char Temp_byte;
    unsigned char i;
    unsigned char j;
    unsigned char Result;

    Temp_byte = addr;

    NSEL = 0;
    NOP;

    for(i = 0; i < 8; i++)
    {
        SDI = (Temp_byte&0x80);
        NOP;
        SCLK = 1;
        NOP;
        SCLK = 0;
        NOP;
        Temp_byte <<= 0x01;
    }


    SDO = 1;//let the sdo pin input
    NOP;

    for (i = 0; i < length; i++)
    {
        Result = 0;
        for(j = 0; j < 8; j++)
        {
            Result = Result<<1;
            SCLK = 1;
            NOP;

            if(SDO)
            {
                Result |= 1;
            }

            SCLK = 0;
            NOP;
        }
        buf[i] = Result;
    }

    NSEL = 1;
    NOP;
}