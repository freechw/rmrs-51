#include "analyze.h"
#include "interseral.h"
#include "lower.h"

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

char Analyze(unsigned char buf[], unsigned long Id, unsigned char result[])
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
    
    if( meterId != Id)
    {
        unsigned char i;
        for (i = 0; i < LOWER_METER_DATA_LENGTH; i++)
        {
            result[i] = 0xff;
        }
        ArrayCopy((unsigned char *)(&Id), result, 0, 4);
        result[37] = WRONG_ID;
        /**************DEBUG****************************/
        InterSendString("analyze:wrong id, recv id is: ");
        InterHexString(&buf[METER_ID_OFFSET], 4);
        InterSendString("\r\n");
        InterSendString("analyze:expect id is ");
        InterHexString((unsigned char *)(&Id), 4);
        InterSendString("\r\n");
        /***********************************************/
        return -1;
    }

    ArrayCopy((unsigned char *)(&meterId), result, 0, 4);
    ArrayCopy((unsigned char *)(&status), result, 4, 1);
    ArrayCopy((unsigned char *)(&histPower), result, 5, 4);
    ArrayCopy((unsigned char *)(&histFlow), result, 9, 4);
    ArrayCopy((unsigned char *)(&runTime), result, 13, 4);
    ArrayCopy((unsigned char *)(&flowTemp), result, 17, 4);
    ArrayCopy((unsigned char *)(&retnTemp), result, 21, 4);
    ArrayCopy((unsigned char *)(&curtPower), result, 25, 4);
    ArrayCopy((unsigned char *)(&curtFlow), result, 29, 4);
    ArrayCopy((unsigned char *)(&dataTime), result, 33, 4);
    result[37] = RIGHT_ID;

    return 0;
}

void ArrayCopy(unsigned char source[], unsigned char target[], unsigned char distIdx, unsigned char num)
{
    unsigned char i;
    for(i = 0; i < num; i++)
    {
        target[distIdx+i] = source[i];
    }
}

char AnalyzeHT(unsigned char buf[])
{
    //unsigned char headByte[] = {0x68, 0xf7, 0xf7, 0x68};
    unsigned long head = 0x68f7f768;
    unsigned char tail = 0x16;

    
    if((head == *(long *)buf) && (tail == buf[252]))
    {
        InterSendString("AnalyzeHT: Package OK!\r\n");
        return 0;
    }
    InterSendString("AnalyzeHT: Package Bad!\r\n");
    return -1;
}
