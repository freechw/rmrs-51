#ifndef __STORE_H__
#define __STORE_H__

void storeReadPage(unsigned char pageNum, unsigned char buf[], unsigned char length) reentrant;
void storeWritePage(unsigned char pageNum, unsigned char buf[], unsigned char length) reentrant;
void StoreInit();
void storeTest(unsigned char ch, unsigned int addr);

void storeWriteTotalNum(unsigned char num) reentrant;
unsigned char storeReadTotalNum() reentrant;

void storeWriteReadNum(unsigned char num) reentrant;
unsigned char storeReadReadNum() reentrant;

void storeWriteTransNum(unsigned char num) reentrant;
unsigned char storeReadTransNum() reentrant;

void storeWriteCommandBuf(unsigned char buf[], unsigned char length) reentrant;
void storeReadCommandBuf(unsigned char buf[], unsigned char length) reentrant;

void storeWriteSi4432Id(unsigned int si4432Id);
unsigned int storeReadSi4432Id();

void storeWriteSyncword(unsigned int syncword);
unsigned int storeReadSyncword();

#endif
