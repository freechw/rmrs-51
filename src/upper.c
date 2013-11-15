#include "upper.h"
#include "si4432.h"
#include "si4432spiport.h"
#include "si4432spi.h"
#include "lower.h"
#include "store.h"
#include "stdlib.h"
#include "interseral.h"

#define STOP_HEX 0x57
#define READ_HEX 0x7d
#define UPLOAD_HEX  0x2d
#define CLEAN_HEX 0xc1

#define UPPER_MAX_SEND_LENGTH LOWER_METER_DATA_LENGTH+4
#define UPPER_COMMAND_LENGTH 20


data unsigned char SI4432IDF = 0x00;
data unsigned char SI4432IDS = 0x00;

data unsigned char upperLowerToRead_Flag = 0;
data unsigned char upperCommandRead_Flag = 0;
data unsigned char upperCommandUpload_Flag= 0;

data unsigned char recvBuf[64];

unsigned char CheckRadioPackage(unsigned char length);
void ResponseSi4432Command(unsigned char buf[], unsigned char length);
void UpperDoReadCommand(unsigned char buf[]);
void UpperDoUploadCommand(unsigned char buf[]);
void UpperDoCleanCommand();

void SendSi4432Package(unsigned char buf[], unsigned char length)
{
    idata unsigned char sendBuf[UPPER_MAX_SEND_LENGTH];
    //data unsigned char tmpEX1; //byte to cache EX1 status;
    sendBuf[0] = SI4432IDF;
    sendBuf[1] = SI4432IDS;
    sendBuf[2] = length;
    MemCopy(&sendBuf[3], &buf[0], length);
    sendBuf[length+3] = STOP_HEX;

    //tmpEX1 = EX1;//cache EX1 status
    //EX1 = 0;//close extern(si4432) inturrupt
    RF_FIFO_Send(sendBuf, (length + 4));
    //EX1 = tmpEX1;//restore EX1 status
}


void Si4432Interrupt() interrupt 2
{
    unsigned char length = 0;
    WDT_CONTR =  0x3F;//feed dog

    IE1 = 0;

    recvBuf[0] = 0x00;
    recvBuf[63] = 0x00;
    RF_Set_IdleMode();

    ItStatus1 = SPI_Read_Reg(0x03);
    ItStatus2 = SPI_Read_Reg(0x04);

    if((ItStatus1 & 0x01) == 0x01)
    {
        //reset RX FIFO
        SPI_Write_Reg(0x08, 0x02);
        SPI_Write_Reg(0x08, 0x00);
        InterSendString("Upper: Si4432 Receive Error!\r\n");
    }
    if((ItStatus1 & 0x02) == 0x02)
    {
        length = SPI_Read_Reg(0x4B);
        SPI_Burst_Read(0x7F, recvBuf, length);

        //check package
        if (1 == CheckRadioPackage(length))
        {
            ResponseSi4432Command(&recvBuf[3], length - 4);
        }
        //clear rx fifo
        SPI_Write_Reg(0x08, 0x02);
        SPI_Write_Reg(0x08, 0x00);
    }

    RF_Set_RXMode();
}

unsigned char CheckRadioPackage(unsigned char length)
{
    //check unit id
    if(SI4432IDF == recvBuf[0] && SI4432IDS == recvBuf[1])
    {
        //check data length
        if((length -4) == recvBuf[2])
        {
            //check stop hex
            if(STOP_HEX == recvBuf[length-1])
            {
                return 1;
            }
            else
            {
                InterSendString("Upper: Si4432 Stop Check Fail!\r\n");
            }
        }
        else
        {
            InterSendString("Upper: Si4432 Length Check Fail!\r\n");
        }
    }
    else
    {
        InterSendString("Upper: Si4432 Address Check Fail!\r\n");
    }
    return 0;
}


void ResponseSi4432Command(unsigned char buf[], unsigned char commandLength)
{
    switch (buf[0])
    {
        case READ_HEX:
            if (1 != lowerReading_Flag)
            {
                //return command ack to concentrator
                SendSi4432Package(buf, commandLength);
                //set the flag to start UpperDoReadCommand()
                UpperDoReadCommand(buf);
            }
            else
            {
                storeReadCommandBuf(buf, UPPER_COMMAND_LENGTH);
                SendSi4432Package(buf, UPPER_COMMAND_LENGTH);
            }
            break;
        case UPLOAD_HEX:
            if(1 == lowerCanTrans_Flag)
            {
                UpperDoUploadCommand(buf);
            }
            else
            {
                storeReadCommandBuf(buf, UPPER_COMMAND_LENGTH);
                SendSi4432Package(buf, UPPER_COMMAND_LENGTH);
            }
            break;
        case CLEAN_HEX:
            SendSi4432Package(buf, commandLength);
            UpperDoCleanCommand();
            break;
        default:
            InterSendString("Upper: Unkonwn Command Hex: ");
            InterHexString(buf, 1);
            InterSendString("\r\n");
    }
}

void UpperDoReadCommand(unsigned char buf[])
{
    unsigned char meterNum;
    unsigned char i;

    InterSendString("Upper: Read Meter Command!\r\n");

    storeWriteCommandBuf(buf, UPPER_COMMAND_LENGTH);
    meterNum = buf[1];
    for ( i = 0; i < meterNum; i++)
    {
        storeWritePage((i), &buf[(i+1) * 4], 4);
    }

    //set the flag to let lower start read cycle
    upperLowerToRead_Flag  = 1;
}

void UpperDoUploadCommand(unsigned char buf[])
{
    //get the meter sequence number in cached command
    unsigned char meterNumber = buf[3] -1;

    InterSendString("Upper: Upload!\r\n");

    //extract the meter data from AT24C256
    storeReadPage(meterNumber, buf, LOWER_METER_DATA_LENGTH);
    //send meter data to concentrator
    SendSi4432Package(buf, LOWER_METER_DATA_LENGTH);
}

void UpperDoCleanCommand()
{
    InterSendString("Upper: Clean!\r\n");
    lowerReading_Flag = 0;
    lowerCanTrans_Flag = 0;
    upperLowerToRead_Flag = 0;
    upperCommandRead_Flag = 0;
    upperCommandUpload_Flag = 0;

}

void UpperInit()
{
    lowerReading_Flag = 0;
    lowerCanTrans_Flag = 0;
    upperLowerToRead_Flag = 0;
    upperCommandRead_Flag = 0;
    upperCommandUpload_Flag = 0;

    SPI_Init();

    RF_Reset();
    RF_Init();

    RF_Set_IdleMode();
    RF_Set_RXMode();

    IT1 = 0;//low triger
    EX1 = 1;//open extern 1(si4432) interrupt

    InterSendString("Upper: Upper Ready!\r\n");
}



