#ifndef __LOWER_H__
#define __LOWER_H__

#define LOWER_METER_DATA_LENGTH 38

//extern bit CD_Flag;//set to 1 when send the read meter seral command
//extern bit RD_Flag;//set to 1 when seral port get the 0x68, it means the start of the meter data package
//extern bit GD_Flag;//set to 1 when get more the 253 bytes from the start(0x68) of the package
extern data char lowerCanTrans_Flag;//set to 1 when read all the recommand meters
extern data char lowerReading_Flag;//set to 1 when read meter cycle reading......

void LowerReadMeterCycle();
void LowerInit();
//void Ch438Interrupt()interrupt 0;

#endif