#include "lower.h"
#include "ch438uart1.h"
#include "ch438direct_simulate.h"
#include "store.h"
#include "analyze.h"
#include "interseral.h"
#include "stdlib.h"
#include <STC/STC12C5A60S2.H>

/************Need Extern Define!!!!!***********

//@ store.h
extern void storeReadPage(unsigned char num, unsigned char buf[], unsigned char length);
extern void storeWritePage(unsigned char num, unsigned char buf[], unsigned char length);
extern unsigned char storeReadTotalNum();
extern void storeWriteReadNum(unsigned char num);
**********************************************/



data unsigned char CD_Flag = 0;//set to 1 when send the read meter seral command
data unsigned char RD_Flag = 0;//set to 1 when seral port get the 0x68, it means the start of the meter data package
data unsigned char GD_Flag = 0;//set to 1 when get more the 253 bytes from the start(0x68) of the package
data unsigned char LowerCanTrans_Flag = 0;//set to 1 when read all the recommand meters
data unsigned char LowerReading_Flag = 0;

data unsigned char ch438ReadNum = 0;//number of vaild bytes read from CH438
xdata unsigned char ch438ReadBuf[128];//buf to store the temp data read from CH438 FIFO

xdata unsigned char Gbuf[255];//buf to store meter data before analyze
data unsigned char GbufCount = 0;//Counter to count the vaild bytes number in Gbuf

xdata unsigned char meterData[LOWER_METER_DATA_LENGTH] = {0};//buf to store meter data after analyze

#define LOWER_READ_TIMEOUT 3 //seconds of read meter timeout


unsigned char ReadMeter(unsigned long id)
{
    unsigned char readCommandLock[] = {0x68, 0x0b, 0x0b, 0x68, 0x53, 0xfd,
                                       0x52, 0x00, 0x00, 0x00, 0x00, 0xff,
                                       0xff, 0xff, 0xff, 0x9e, 0x16};
    unsigned char readCommandRead[] = {0x10, 0x5b, 0xfd, 0x58, 0x16};

    unsigned char reReadCount = 3;

    /*************DEBUG***************/
    InterSendString("Lower:Read Meter! Id is 0x");
    InterHexString((unsigned char *)&id, 4);
    InterSendString("\r\n");
    /*********************************/

    WDT_CONTR =  0x3F;//feed dog
    Ch438OpenInterrupt();

    *(long *)(&readCommandLock[7]) = id;
    readCommandLock[15] = readCommandLock[15]+ readCommandLock[7] + readCommandLock[8] + readCommandLock[9] + readCommandLock[10];


    while(reReadCount > 0)
    {
        WDT_CONTR =  0x3F;//feed dog
        reReadCount--;
        CD_Flag = 1;
        RD_Flag = 0;
        GD_Flag = 0;
        TimerLower_Flag = 0;
        Ch438Uart1SendBuf(readCommandLock, 17);
        Delay100ms();
        Ch438Uart1SendBuf(readCommandRead, 5);
        TimerLower(LOWER_READ_TIMEOUT);
        while((GD_Flag == 0)&&(TimerLower_Flag == 0));
        TimerLowerOff();
        if(1 == GD_Flag)
        {
            if(1 == AnalyzeHT(Gbuf))
            {
                Ch438CloseInterrupt();
                return 1;
            }
        }
        else
        {
            //continue;
        }
        Gbuf[0] = 0x00;
        Gbuf[252] = 0x00;
    }
    Ch438CloseInterrupt();
    return 0;
}


void LowerReadMeterCycle()
{
    unsigned char totalNum;
    unsigned char i;
    LowerReading_Flag = 1;
    totalNum = storeReadTotalNum();

    for ( i = 0; i < totalNum; i++)
    {
        unsigned long id;

        storeReadPage(i, (char *)(&id), 4);
        if (1 == ReadMeter(id))
        {
            WDT_CONTR =  0x3F;//feed dog
            if (1 == Analyze(Gbuf, id, meterData))
            {
                storeWritePage(i, meterData, LOWER_METER_DATA_LENGTH);
            }
            else
            {
                storeWritePage(i, meterData, LOWER_METER_DATA_LENGTH);
            }
        }
        else
        {
            unsigned char tmpi;
            WDT_CONTR =  0x3F;//feed dog
            for (tmpi = 0; tmpi < LOWER_METER_DATA_LENGTH; tmpi++)
            {
                meterData[tmpi] = 0;
            }
            *(long *)(&meterData[0]) = id;
            meterData[LOWER_METER_DATA_LENGTH-1] = 0x07;
            storeWritePage(i, meterData, LOWER_METER_DATA_LENGTH);
        }
        storeWriteReadNum(i+1);
        LowerCanTrans_Flag = 1;
    }
}


void Ch438Interrupt() interrupt 0
{
    unsigned char gInterruptStatus;
    unsigned char interruptStatus;
    unsigned char trash;

    EA = 0;

//    InterSendString("CH438:interrupt!\r\n");

    gInterruptStatus = simRead(SSR);
//    InterSendString("CH438:SSR is ");
//    InterHexString(&gInterruptStatus, 1);
//    InterSendString("\r\n");

    if(0x02 == (0x02 & gInterruptStatus))
    {
        interruptStatus = (~(0xc0) & simRead(IIR1));

//        InterSendString("CH438:IIR1 is ");
//        InterHexString(&interruptStatus, 1);
//        InterSendString("\r\n");


        switch(interruptStatus)
        {
            case 0x01:
                break;
            case 0x06:
//                InterSendString("CH438: LSR Error!LSR:");
                trash = simRead(LSR1);
//                InterHexString(&trash, 1);
//                InterSendString("\r\n");
                break;
            case 0x0c:
            case 0x04:
                //interSendString("CH438: RHR!\r\n");
                //interSendString("CH438: test\r\n");
                ch438ReadNum = Ch438Uart1Read(ch438ReadBuf);
                //interSendString("CH438: recevd\r\n");
                //interSend(ch438ReadBuf, readNum);
                if (1 == CD_Flag)
                {
                    if ( 1 != RD_Flag)
                    {
                        unsigned char i = 0;
                        while ( 0x68 != ch438ReadBuf[i])
                        {
                            i++;
                            if ( i >= ch438ReadNum)
                            {
                                break;
                            }
                        }
                        if( i >= ch438ReadNum)
                        {
                            //nothing
                        }
                        else
                        {
                            RD_Flag = 1;
                            GbufCount = ch438ReadNum - i;
                            MemCopy(&Gbuf[0], &ch438ReadBuf[i], GbufCount);
                        }
                    }
                    else
                    {
                        MemCopy(&Gbuf[GbufCount], &ch438ReadBuf[0], ch438ReadNum);
                        GbufCount = GbufCount + ch438ReadNum;
                        if( GbufCount >= 253)
                        {
                            CD_Flag = 0;
                            RD_Flag = 0;
                            GD_Flag = 1;
                        }
                    }
                }
                break;
            case 0x02:
//                InterSendString("CH438: THR Empty!IIR1:");
                trash = simRead(IIR1);
//                InterHexString(&trash, 1);
//                InterSendString("\r\n");
                break;
            case 0x00:
//                InterSendString("CH438: MSR Error!MSR1:");
                trash = simRead(MSR1);
//                InterHexString(&trash, 1);
//                InterSendString("\r\n");
                break;
            default:
                break;
        }
    }
    if (0x00 != (0xfd & gInterruptStatus))
    {
        InterSendString("CH438: Other Uart Interrupt!\r\n");
        trash = simRead(LSR0);
        trash = simRead(LSR2);
        trash = simRead(LSR3);
        trash = simRead(LSR4);
        trash = simRead(LSR5);
        trash = simRead(LSR6);
        trash = simRead(LSR7);

        trash = simRead(MSR0);
        trash = simRead(MSR2);
        trash = simRead(MSR3);
        trash = simRead(MSR4);
        trash = simRead(MSR5);
        trash = simRead(MSR6);
        trash = simRead(MSR7);

        trash = simRead(IIR0);
        trash = simRead(IIR2);
        trash = simRead(IIR3);
        trash = simRead(IIR4);
        trash = simRead(IIR5);
        trash = simRead(IIR6);
        trash = simRead(IIR7);

        trash = simRead(RBR0);
        trash = simRead(RBR2);
        trash = simRead(RBR3);
        trash = simRead(RBR4);
        trash = simRead(RBR5);
        trash = simRead(RBR6);
        trash = simRead(RBR7);

        simWrite(THR0, trash);
        simWrite(THR2, trash);
        simWrite(THR3, trash);
        simWrite(THR4, trash);
        simWrite(THR5, trash);
        simWrite(THR6, trash);
        simWrite(THR7, trash);
    }
    EA = 1;
}

void LowerInit()
{
    InitCh438Uart1();

//    Ch438Uart1SendString("hello meter!\r\n");

    CD_Flag = 0;
    RD_Flag = 0;
    GD_Flag = 0;

    EX0 = 0;
    IT0 = 0;

    InterSendString("Lower: Lower Ready!\r\n");
}