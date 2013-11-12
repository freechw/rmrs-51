#include "ch438direct_simulate.h"
#include "STC/STC12C5A60S2.H>
#include <intrins.h>

#define NOP _nop_();_nop_();_nop_()

unsigned char simRead(unsigned char addr) small
{
    unsigned char value;
    P2 = 0xff;
    NOP;
    P0 = 0xff;
    NOP;
    P2 = addr;
    NOP;
    WR = 1;
    NOP;
    RD = 0;
    NOP;

    NOP;
    NOP;

    value = P0;

    RD = 1;
    NOP;
    P2 = 0xff;
    NOP;
    P0 = 0xff;

    return value;
}

void simWrite(unsigned char addr, unsigned char value) small
{
    P2 = 0xff;
    NOP;
    P2 = addr;
    NOP;
    P0 = 0xff;
    NOP;
    P0 = value;
    NOP;
    NOP;
    WR = 0;
    NOP;

    NOP;
    WR = 1;
    NOP;
    P0 = 0xff;
    NOP;
    P2 = 0xff;
    NOP;
}
