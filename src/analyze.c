#include "analyze.h"
#include "interseral.h"
#include "lower.h"
#include "stdlib.h"

#define METER_ID_OFFSET 7
#define STATUS_OFFSET 16
#define HISTPOWER_OFFSET 27
#define HISTFLOW_OFFSET 33
#define RUNTIME_OFFSET 39
#define FLOWTEMP_OFFSET 45
#define RETNTEMP_OFFSET 51
#define CURTPOWER_OFFSET 63
#define CURTFLOW_OFFSET 75
#define DATA_TIME_OFFSET 124
#define WRONG_ID 0x7d
#define RIGHT_ID 0x00

#define METER_DATA_HEAD 0x68
#define METER_DATA_TAIL 0x16

unsigned char Analyze(unsigned char buf[], unsigned long Id, unsigned char result[])
{
    unsigned long meterId = *(unsigned long *)(&buf[METER_ID_OFFSET]);
    unsigned char status = buf[STATUS_OFFSET];
    unsigned long histPower = *(unsigned long *)(&buf[HISTPOWER_OFFSET]);
    unsigned long histFlow = *(unsigned long *)(&buf[HISTFLOW_OFFSET]);
    unsigned long runTime = *(unsigned long *)(&buf[RUNTIME_OFFSET]);
    unsigned long flowTemp = *(unsigned long *)(&buf[FLOWTEMP_OFFSET]);
    unsigned long retnTemp = *(unsigned long *)(&buf[RETNTEMP_OFFSET]);
    unsigned long curtPower = *(unsigned long *)(&buf[CURTPOWER_OFFSET]);
    unsigned long curtFlow = *(unsigned long *)(&buf[CURTFLOW_OFFSET]);
    unsigned long dataTime = *(unsigned long *)(&buf[DATA_TIME_OFFSET]);
    
//    if( meterId != Id)
//    {
//        unsigned char i;
//        for (i = 0; i < LOWER_METER_DATA_LENGTH; i++)
//        {
//            result[i] = 0xff;
//        }
//        ArrayCopy((unsigned char *)(&Id), result, 0, 4);
//        result[37] = WRONG_ID;
//        /**************DEBUG****************************/
//        InterSendString("analyze:wrong id, recv id is: ");
//        InterHexString(&buf[METER_ID_OFFSET], 4);
//        InterSendString("\r\n");
//        InterSendString("analyze:expect id is ");
//        InterHexString((unsigned char *)(&Id), 4);
//        InterSendString("\r\n");
//        /***********************************************/
//        return 0;
//    }

    MemCopy(&result[0], (unsigned char *)(&Id), sizeof(Id));
    MemCopy(&result[4], (unsigned char *)(&status), sizeof(status));
    MemCopy(&result[5], (unsigned char *)(&histPower), sizeof(histPower));
    MemCopy(&result[9], (unsigned char *)(&histFlow), sizeof(histFlow));
    MemCopy(&result[13], (unsigned char *)(&runTime), sizeof(runTime));
    MemCopy(&result[17], (unsigned char *)(&flowTemp), sizeof(flowTemp));
    MemCopy(&result[21], (unsigned char *)(&retnTemp), sizeof(retnTemp));
    MemCopy(&result[25], (unsigned char *)(&curtPower), sizeof(curtPower));
    MemCopy(&result[29], (unsigned char *)(&curtFlow), sizeof(curtFlow));
    MemCopy(&result[33], (unsigned char *)(&dataTime), sizeof(dataTime));
    result[37] = RIGHT_ID;

    return 1;
}

unsigned char AnalyzeHT(unsigned char buf[])
{
    //unsigned char headByte[] = {0x68, 0xf7, 0xf7, 0x68};

    if((METER_DATA_HEAD == buf[0]) && (METER_DATA_TAIL == buf[252]))
    {
        InterSendString("AnalyzeHT: Package OK!\r\n");
        return 1;
    }
    InterSendString("AnalyzeHT: Package Bad!\r\n");
    return 0;
}
