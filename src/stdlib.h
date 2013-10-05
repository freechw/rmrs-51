#ifndef __STDLIB_H__
#define __STDLIB_H__

extern bit TimerLower_Flag;//set to 1 when the timer clock;
extern bit TimerUpper_Flag;
void AddBuf(unsigned char target[], unsigned char target_idx, unsigned char source[], unsigned char  source_idx, unsigned char n)  reentrant;

#define LowerInterruptAddBuf AddBuf
#define UpperInterruptAddBuf AddBuf
//void UpperInterruptAddBuf(unsigned char target[], unsigned char target_idx, unsigned char source[], unsigned char  source_idx, unsigned char n);
//void LowerInterruptAddBuf(unsigned char target[], unsigned char target_idx, unsigned char source[], unsigned char  source_idx, unsigned char n);
void TimersInit();
void TimerLower(unsigned char seconds);
void TimerLowerOff();
void TimerUpper(unsigned char seconds);
void TimerUpperOff();
void Delay100ms();

#endif