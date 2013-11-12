#include "stdlib.h"
#include "interseral.h"
#include <STC/STC12C5A60S2.H>

data unsigned int timerCountLower = 0;//Timer current count number
data unsigned char timerAlarmNumLower = 0;//Timer setting time
data unsigned char TimerLower_Flag = 0;//set to 1 when the timer clock;

data unsigned int timerCountUpper = 0;//Timer current count number
data unsigned char timerAlarmNumUpper = 0;//Timer setting time
data unsigned char TimerUpper_Flag = 0;//set to 1 when the timer clock;




void MemCopy(unsigned char target[], unsigned char source[], unsigned char n) reentrant
{
    unsigned char i;
    for( i = 0; i < n; i++)
    {
        target[i] = source[i];
    }
}

void TimersInit()
{
//    AUXR &= 0x7F;
//    TMOD &= 0xF0;
//    TMOD &= 0x01;

//    AUXR &= 0xBF;
//    TMOD &= 0x0F;
//    TMOD &= 0x10;
}

void TimerLower(unsigned char seconds)
{
    timerAlarmNumLower = seconds;
    timerCountLower = 0;
    TimerLower_Flag = 0;

    AUXR &= 0x7F;
    TMOD &= 0xF0;
    TMOD |= 0x01;

    TL0 = 0x00;
    TH0 = 0x04;
    TF0 = 0;

    ET0 = 1;
    EA = 1;
    TR0 = 1;
}

void TimerLowerOff()
{
    TR0 = 0;
    timerAlarmNumLower = 0;
    timerCountLower = 0;
    ET0 = 0;
}


void TimerUpper(unsigned char seconds)
{
    timerAlarmNumUpper = seconds;
    timerCountUpper = 0;
    TimerUpper_Flag = 0;

    AUXR &= 0xBF;
    TMOD &= 0x0F;
    TMOD |= 0x10;

    TL1 = 0x00;
    TH1 = 0x04;
    TF1 = 0;

    ET1 = 1;
    EA = 1;
    TR1 = 1;
    }

void TimerUpperOff()
{
    TR1 = 0;
    timerAlarmNumUpper = 0;
    timerCountUpper = 0;
    ET1 = 0;
}

void timerLowerinturrupt() interrupt 1
{
    //InterSendString("Timer Lower!\r\n");
    ET0 = 0;
    TF0 = 0;
    timerCountLower++;
    if ( timerCountLower >= (timerAlarmNumLower * 14))
    {
        InterSendString("TimerLower: time out!\r\n");
        timerCountLower = 0;
        TimerLower_Flag = 1;
        TR0 = 0;
    }
    else
    {
        TL0 = 0x00;
        TH0 = 0x04;
    }
    ET0 = 1;
}

void timerUpperinturrupt() interrupt 3
{
    //InterSendString("Timer Upper!\r\n");
    ET1 = 0;
    TF1 = 0;
    timerCountUpper++;
    if ( timerCountUpper >= (timerAlarmNumUpper * 14))
    {
        InterSendString("TimerUpper: time out!\r\n");
        timerCountUpper = 0;
        TimerUpper_Flag = 1;
        TR1 = 0;
    }
    else
    {
        TL1 = 0x00;
        TH1 = 0x04;
    }
    ET1 = 1;
}

void Delay100ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 5;
	j = 52;
	k = 195;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


