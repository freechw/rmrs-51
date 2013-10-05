#ifndef _DIRECT_H_
#define _DIRECT_H_

extern volatile unsigned char xdata SSR;

extern volatile unsigned char xdata RBR1;
#define THR1 RBR1
extern volatile unsigned char xdata IER1;
extern volatile unsigned char xdata IIR1;
#define FCR1 IIR1
extern volatile unsigned char xdata LCR1;
extern volatile unsigned char xdata MCR1;
extern volatile unsigned char xdata LSR1;
extern volatile unsigned char xdata MSR1;
extern volatile unsigned char xdata SCR1;
#define DLL1 RBR1
#define DLM1 IER1

extern volatile unsigned char xdata RBR0;
#define THR0 RBR0
extern volatile unsigned char xdata IER0;
extern volatile unsigned char xdata IIR1;
#define FCR0 IIR0
extern volatile unsigned char xdata LCR0;
extern volatile unsigned char xdata MCR0;
extern volatile unsigned char xdata LSR0;
extern volatile unsigned char xdata MSR0;
extern volatile unsigned char xdata SCR0;
#define DLL0 RBR0
#define DLM0 IER0

extern volatile unsigned char xdata RBR2;
#define THR2 RBR2
extern volatile unsigned char xdata IER2;
extern volatile unsigned char xdata IIR1;
#define FCR2 IIR2
extern volatile unsigned char xdata LCR2;
extern volatile unsigned char xdata MCR2;
extern volatile unsigned char xdata LSR2;
extern volatile unsigned char xdata MSR2;
extern volatile unsigned char xdata SCR2;
#define DLL2 RBR2
#define DLM2 IER2

extern volatile unsigned char xdata RBR3;
#define THR3 RBR3
extern volatile unsigned char xdata IER3;
extern volatile unsigned char xdata IIR1;
#define FCR3 IIR3
extern volatile unsigned char xdata LCR3;
extern volatile unsigned char xdata MCR3;
extern volatile unsigned char xdata LSR3;
extern volatile unsigned char xdata MSR3;
extern volatile unsigned char xdata SCR3;
#define DLL3 RBR3
#define DLM3 IER3

extern volatile unsigned char xdata RBR4;
#define THR4 RBR4
extern volatile unsigned char xdata IER4;
extern volatile unsigned char xdata IIR1;
#define FCR4 IIR4
extern volatile unsigned char xdata LCR4;
extern volatile unsigned char xdata MCR4;
extern volatile unsigned char xdata LSR4;
extern volatile unsigned char xdata MSR4;
extern volatile unsigned char xdata SCR4;
#define DLL4 RBR4
#define DLM4 IER4

extern volatile unsigned char xdata RBR5;
#define THR5 RBR5
extern volatile unsigned char xdata IER5;
extern volatile unsigned char xdata IIR1;
#define FCR5 IIR5
extern volatile unsigned char xdata LCR5;
extern volatile unsigned char xdata MCR5;
extern volatile unsigned char xdata LSR5;
extern volatile unsigned char xdata MSR5;
extern volatile unsigned char xdata SCR5;
#define DLL5 RBR5
#define DLM5 IER5

extern volatile unsigned char xdata RBR6;
#define THR6 RBR6
extern volatile unsigned char xdata IER6;
extern volatile unsigned char xdata IIR1;
#define FCR6 IIR6
extern volatile unsigned char xdata LCR6;
extern volatile unsigned char xdata MCR6;
extern volatile unsigned char xdata LSR6;
extern volatile unsigned char xdata MSR6;
extern volatile unsigned char xdata SCR6;
#define DLL6 RBR6
#define DLM6 IER6

extern volatile unsigned char xdata RBR7;
#define THR7 RBR7
extern volatile unsigned char xdata IER7;
extern volatile unsigned char xdata IIR1;
#define FCR7 IIR7
extern volatile unsigned char xdata LCR7;
extern volatile unsigned char xdata MCR7;
extern volatile unsigned char xdata LSR7;
extern volatile unsigned char xdata MSR7;
extern volatile unsigned char xdata SCR7;
#define DLL7 RBR7
#define DLM7 IER7



#endif