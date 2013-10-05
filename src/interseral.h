#ifndef __INTERSERAL_H__
#define __INTERSERAL_H__

extern bit interSeralRecv_Flag;

void InterSeralInit();
void InterSendByte(unsigned char byte);
unsigned char InterRecvByte();
void InterSend(unsigned char * buf, unsigned char length);
void InterSendString(unsigned char * buf);
void InterHexString(unsigned char buf[], unsigned char length);

#endif