#ifndef __CH438UART1_H__
#define __CH438UART1_H__

void InitCh438Uart1();
void Ch438Uart1SendByte(unsigned char byte);
void Ch438Uart1SendBuf(unsigned char buf[], unsigned char length);
unsigned char Ch438Uart1Read(unsigned char * buf);
void Ch438Uart1SendString(unsigned char * string);
void Ch438Uart1Send(unsigned char * buf, unsigned char length);

#endif