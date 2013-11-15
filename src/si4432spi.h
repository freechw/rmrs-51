#ifndef __SI4432SPI_H__
#define __SI4432SPI_H__

void SPI_Init();
void SPI_Write_Reg(unsigned char reg, unsigned char value);
unsigned char SPI_Read_Reg(unsigned char reg);
void SPI_Burst_Write(unsigned char addr, unsigned char buf[], unsigned char length);
void SPI_Burst_Read(unsigned char addr, unsigned char buf[], unsigned char length);

#endif