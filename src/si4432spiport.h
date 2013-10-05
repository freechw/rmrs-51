#ifndef __SI4432SPIPORT_H__
#define __SI4432SPIPORT_H__

#include <STC/STC12C5A60S2.H>

sbit NSEL = P1^1;
sbit SCLK = P1^7;
sbit SDI =  P1^5;
sbit SDO =  P1^6;
sbit NIRQ = P3^3;
sbit SDN =  P1^0;

#endif