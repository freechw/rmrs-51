#include "ch438regaddress.h"

volatile unsigned char xdata SSR _at_ 0x4f00;
volatile unsigned char xdata RBR1 _at_ 0x9000;
//#define THR1 RBR1
volatile unsigned char xdata IER1 _at_ 0x9100;
volatile unsigned char xdata IIR1 _at_ 0x9200;
//#define FCR1 IIR1
volatile unsigned char xdata LCR1 _at_ 0x9300;
volatile unsigned char xdata MCR1 _at_ 0x9400;
volatile unsigned char xdata LSR1 _at_ 0x9500;
volatile unsigned char xdata MSR1 _at_ 0x9600;
volatile unsigned char xdata SCR1 _at_ 0x9700;
//#define DLL1 RBR1
//#define DLM1 IER1


volatile unsigned char xdata RBR0 _at_ 0x8000;
//#define THR0 RBR0
volatile unsigned char xdata IER0 _at_ 0x8100;
volatile unsigned char xdata IIR0 _at_ 0x8200;
//#define FCR0 IIR0
volatile unsigned char xdata LCR0 _at_ 0x8300;
volatile unsigned char xdata MCR0 _at_ 0x8400;
volatile unsigned char xdata LSR0 _at_ 0x8500;
volatile unsigned char xdata MSR0 _at_ 0x8600;
volatile unsigned char xdata SCR0 _at_ 0x8700;
//#define DLL0 RBR0
//#define DLM0 IER0

volatile unsigned char xdata RBR2 _at_ 0xa000;
//#define THR2 RBR2
volatile unsigned char xdata IER2 _at_ 0xa100;
volatile unsigned char xdata IIR2 _at_ 0xa200;
//#define FCR2 IIR2
volatile unsigned char xdata LCR2 _at_ 0xa300;
volatile unsigned char xdata MCR2 _at_ 0xa400;
volatile unsigned char xdata LSR2 _at_ 0xa500;
volatile unsigned char xdata MSR2 _at_ 0xa600;
volatile unsigned char xdata SCR2 _at_ 0xa700;
//#define DLL2 RBR2
//#define DLM2 IER2

volatile unsigned char xdata RBR3 _at_ 0xb000;
//#define THR3 RBR3
volatile unsigned char xdata IER3 _at_ 0xb100;
volatile unsigned char xdata IIR3 _at_ 0xb200;
//#define FCR3 IIR3
volatile unsigned char xdata LCR3 _at_ 0xb300;
volatile unsigned char xdata MCR3 _at_ 0xb400;
volatile unsigned char xdata LSR3 _at_ 0xb500;
volatile unsigned char xdata MSR3 _at_ 0xb600;
volatile unsigned char xdata SCR3 _at_ 0xb700;
//#define DLL3 RBR3
//#define DLM3 IER3

volatile unsigned char xdata RBR4 _at_ 0xc000;
//#define THR4 RBR4
volatile unsigned char xdata IER4 _at_ 0xc100;
volatile unsigned char xdata IIR4 _at_ 0xc200;
//#define FCR4 IIR4
volatile unsigned char xdata LCR4 _at_ 0xc300;
volatile unsigned char xdata MCR4 _at_ 0xc400;
volatile unsigned char xdata LSR4 _at_ 0xc500;
volatile unsigned char xdata MSR4 _at_ 0xc600;
volatile unsigned char xdata SCR4 _at_ 0xc700;
//#define DLL4 RBR4
//#define DLM4 IER4

volatile unsigned char xdata RBR5 _at_ 0xd000;
//#define THR5 RBR5
volatile unsigned char xdata IER5 _at_ 0xd100;
volatile unsigned char xdata IIR5 _at_ 0xd200;
//#define FCR5 IIR5
volatile unsigned char xdata LCR5 _at_ 0xd300;
volatile unsigned char xdata MCR5 _at_ 0xd400;
volatile unsigned char xdata LSR5 _at_ 0xd500;
volatile unsigned char xdata MSR5 _at_ 0xd600;
volatile unsigned char xdata SCR5 _at_ 0xd700;
//#define DLL5 RBR5
//#define DLM5 IER5

volatile unsigned char xdata RBR6 _at_ 0xe000;
//#define THR6 RBR6
volatile unsigned char xdata IER6 _at_ 0xe100;
volatile unsigned char xdata IIR6 _at_ 0xe200;
//#define FCR6 IIR6
volatile unsigned char xdata LCR6 _at_ 0xe300;
volatile unsigned char xdata MCR6 _at_ 0xe400;
volatile unsigned char xdata LSR6 _at_ 0xe500;
volatile unsigned char xdata MSR6 _at_ 0xe600;
volatile unsigned char xdata SCR6 _at_ 0xe700;
//#define DLL6 RBR6
//#define DLM6 IER6

volatile unsigned char xdata RBR7 _at_ 0xf000;
//#define THR7 RBR7
volatile unsigned char xdata IER7 _at_ 0xf100;
volatile unsigned char xdata IIR7 _at_ 0xf200;
//#define FCR7 IIR7
volatile unsigned char xdata LCR7 _at_ 0xf300;
volatile unsigned char xdata MCR7 _at_ 0xf400;
volatile unsigned char xdata LSR7 _at_ 0xf500;
volatile unsigned char xdata MSR7 _at_ 0xf600;
volatile unsigned char xdata SCR7 _at_ 0xf700;
//#define DLL7 RBR7
//#define DLM7 IER7

