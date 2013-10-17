#ifndef __CH438DIRECT_SIMULATE_H__
#define __CH438DIRECT_SIMULATE_H__

#include <STC/STC12C5A60S2.H>
#include "ch438direct_simulate_regaddrs.h"

sbit WR = P3^6;
sbit RD = P3^7;

unsigned char simRead(unsigned char addr);
void simWrite(unsigned char addr, unsigned char value);


#endif