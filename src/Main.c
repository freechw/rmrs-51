#include <STC/STC12C5A60S2.H>
#include "interseral.h"
#include "stdlib.h"
#include <intrins.h>

#include "upper.h"
#include "lower.h"
#include "store.h"
#include "ch438uart1.h"
//#include "ch438regaddress.h"
//#include "ch438direct_simulate.h"

//for timer 1 interrupt
//#include "si4432spiport.h"


#define NOP; _nop_();

void Delay100ms();
void Timer1Init();
void SetInterruptPriority();
void WaitCommand();

void main(void)
{
    EA = 0;
    IE = 0x00;
    IE2 = 0x00;

    InterSeralInit();
    TimersInit();

    SetInterruptPriority();

    InterSendString("Main: System Up!\r\n");

    WaitCommand();

    UpperInit();

    LowerInit();

    StoreInit();

    EA = 1;

    TimerLowerOff();
    TimerUpperOff();

    //main loop
    while(1)
    {
        //InterSendString("main loop!\r\n");
//        if (1 == upperCommandRead_Flag)
//        {
//            UpperDoReadCommand();
//            upperCommandRead_Flag = 0;
//        }
        if (1 == upperLowerToRead_Flag)
        {
            LowerReadMeterCycle();
            upperLowerToRead_Flag = 0;
        }
    }
}



void SetInterruptPriority()
{
    /* set the extern 0(ch438) interrupt highest,
       timer 0(TimerLower) interrupt middle,
       timer 1(TimerUpper) interrupt middle,
       and extern 1(si4432) interrupt lowest.
    */
    IPH = 0x0B;
    IP = 0x05;
}

void WaitCommand()
{
    unsigned int tmpInt;
    TimerUpper(2);
    while(0 == TimerUpper_Flag)
    {
        unsigned char byte = InterRecvByte();
        if (1 == interSeralRecv_Flag)
        {
            interSeralRecv_Flag = 0;
            //change unit id
            if ( 0x1d == byte)
            {
                unsigned int si4432Id;
                TimerUpperOff();

                InterSendString("change unit id mode!\r\n");
                InterSendString("please enter first byte of unit id!\r\n");
                while (0 == interSeralRecv_Flag)
                {
                    byte = InterRecvByte();
                }
                SI4432IDS = byte;
                interSeralRecv_Flag = 0;

                InterSendString("please enter second byte of unit id!\r\n");
                while (0 == interSeralRecv_Flag)
                {
                    byte = InterRecvByte();
                }
                SI4432IDF = byte;
                interSeralRecv_Flag = 0;

                si4432Id = (SI4432IDF << 8) | (SI4432IDS);
                storeWriteSi4432Id(si4432Id);
                break;
            }
            //change sync word
            if ( 0x5d == byte)
            {
                unsigned int syncword;
                TimerUpperOff();

                InterSendString("change sync word mode!\r\n");
                InterSendString("please enter first byte of syncword!\r\n");
                while (0 == interSeralRecv_Flag)
                {
                    byte = InterRecvByte();
                }
                SYNCWORDF = byte;
                interSeralRecv_Flag = 0;

                InterSendString("please enter second byte of syncword!\r\n");
                while (0 == interSeralRecv_Flag)
                {
                    byte = InterRecvByte();
                }
                SYNCWORDS = byte;
                interSeralRecv_Flag = 0;

                syncword = (SYNCWORDF << 8) | (SYNCWORDS);
                storeWriteSyncword(syncword);
                break;
            }
        }
        interSeralRecv_Flag = 0;
    }
    TimerUpperOff();
    tmpInt = storeReadSi4432Id();
    SI4432IDS = 0xff & (tmpInt);
    SI4432IDF = (tmpInt >> 8);
    tmpInt = storeReadSyncword();
    SYNCWORDS = 0xff & (tmpInt);
    SYNCWORDF = (tmpInt >> 8);
    InterSendString("unit id is: ");
    InterHexString(&SI4432IDS, 1);
    InterHexString(&SI4432IDF, 1);
    InterSendString("\r\n");
    InterSendString("sync word is ");
    InterHexString(&SYNCWORDF, 1);
    InterHexString(&SYNCWORDS, 1);
    InterSendString("\r\n");

}
