#ifndef __SI4432_H__
#define __SI4432_H__

extern data unsigned char ItStatus1,ItStatus2;

extern data unsigned char SYNCWORDF;
extern data unsigned char SYNCWORDS;

void RF_Init();

void RF_Set_RXMode(void);

void RF_Set_TXMode(void);

void RF_Set_IdleMode(void);

void RF_FIFO_Send(unsigned char buf[], unsigned char length);

void RF_Reset();

#endif