#include <STC/STC12C5A60S2.H>
#include "interseral.h"
#include "stdlib.h"
//#include <intrins.h>

#include "upper.h"
#include "lower.h"
#include "store.h"

#include "si4432.h"


//#define NOP; _nop_();

void SetInterruptPriority();
void WaitCommand();
void IdleProcess();

data unsigned short idleCount1 = 0;
data unsigned char idleCount2 = 60;

void main(void)
{
    WDT_CONTR =  0x2F;//watch dog enble and hold count in idle mode(this project doesn't use idle mode)
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

    idleCount1 = 0;
    idleCount2 = 60;

    WDT_CONTR =  0x3F;//feed dog

    //main loop
    while(1)
    {
        if (1 == upperLowerToRead_Flag)
        {
            WDT_CONTR =  0x3F;//feed dog
            LowerReadMeterCycle();
            upperLowerToRead_Flag = 0;
            WDT_CONTR =  0x3F;//feed dog
        }
        IdleProcess();
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
    //unsigned char * dataPoint;
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
    //dataPoint = &SI4432IDS;
    InterHexString(&SI4432IDS, 1);
    //dataPoint = &SI4432IDF;
    InterHexString(&SI4432IDS, 1);
    InterSendString("\r\n");
    InterSendString("sync word is ");
    InterHexString(&SYNCWORDF, 1);
    InterHexString(&SYNCWORDS, 1);
    InterSendString("\r\n");

}

void IdleProcess()
{
    idleCount1--;
    if (0 == idleCount1)
    {
        idleCount2--;
        WDT_CONTR =  0x3F;//feed dog
    }
    if (0 == idleCount2)
    {
        idleCount2 = 60;
        InterSendString("Idle Process: Version: 0.9  Working......\r\n");
    }
} 
