#ifndef __SI4432SPI_H__
#define __SI4432SPI_H__

void SPI_Write_Reg(unsigned char reg, unsigned char value) small;
unsigned char SPI_Read_Reg(unsigned char reg) small;
void SPI_Burst_Write(unsigned char addr, unsigned char buf[], unsigned char length) small;
void SPI_Burst_Read(unsigned char addr, unsigned char buf[], unsigned char length) small;

#endif