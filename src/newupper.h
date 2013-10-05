#ifndef __NEWUPPER_H__
#define __NEWUPPER_H__


extern data char upperCommandRead_Flag;
extern data char upperCommandUpload_Flag;
extern data char upperLowerToRead_Flag;

extern data unsigned char SI4432IDF;
extern data unsigned char SI4432IDS;

extern data unsigned char SYNCWORDF;
extern data unsigned char SYNCWORDS;

void UpperInit();
void UpperDoReadCommand();
void UpperDoUploadCommand();

void Si4432Interrupt();

#endif