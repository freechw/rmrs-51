#ifndef __SI4432_H__
#define __SI4432_H__

extern data unsigned char ItStatus1,ItStatus2;

void RF_init(unsigned char syncwordf, unsigned char syncwords);

void RF_Set_RXMode(void);

void RF_Set_TXMode(void);

void RF_Set_IdleMode(void);

void RF_FIFO_Send(unsigned char buf[], unsigned char length);

void RF_Reset();

#endif