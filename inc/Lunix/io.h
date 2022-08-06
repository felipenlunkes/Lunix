
#ifndef IO_H
#define IO_H

void halt(void);
void disable(void);
void enable(void);
void nop(void);
void outportb(unsigned short port, unsigned char val);
void outportw(unsigned short port, unsigned short val);
unsigned char inportb(unsigned short port);
void HaltProcessor(void);
void wait();

struct IDTEncode
{
	unsigned long base;
	unsigned long segment;
	unsigned char flags;
};

struct IDTEntry
{
	unsigned short LowBase;
	unsigned short segment;
	unsigned char zero;
	unsigned char flags;
	unsigned short HighBase;
} __attribute__((packed));

struct IDTLoc
{
	unsigned short limit;
	unsigned int base;
} __attribute__((packed));

extern void reloadGDT();
void EncodeIDTEntry(int, struct IDTEncode*);
void setupIDT();
extern void loadIDT();


void remapPIC();
extern void ISR0(); /* Division by zero exception */
void CISR0(unsigned int, unsigned int, unsigned int);
extern void ISR1(); /* Debug exception */
void CISR1(unsigned int, unsigned int, unsigned int);
extern void ISR2(); /* NMI exception */
void CISR2(unsigned int, unsigned int, unsigned int);
extern void ISR3(); /* Breakpoint exception */
void CISR3(unsigned int, unsigned int, unsigned int);
extern void ISR4(); /* Overflow exception */
void CISR4(unsigned int, unsigned int, unsigned int);
extern void ISR5(); /* Out of bounds exception */
void CISR5(unsigned int, unsigned int, unsigned int);
extern void ISR6(); /* Invalid opcode exception */
void CISR6(unsigned int, unsigned int, unsigned int);
extern void ISR7(); /* No coprocessor exception */
void CISR7(unsigned int, unsigned int, unsigned int);
extern void ISR8(); /* Double fault exception */
void CISR8(unsigned int, unsigned int, unsigned int, unsigned int);
extern void ISR9(); /* Coprocessor segment overrun exception */
void CISR9(unsigned int, unsigned int, unsigned int);
extern void ISR10(); /* Bad TSS exception */
void CISR10(unsigned int, unsigned int, unsigned int, unsigned int);
extern void ISR11(); /* Segment not present exception */
void CISR11(unsigned int, unsigned int, unsigned int, unsigned int);
extern void ISR12(); /* Stack fault exception */
void CISR12(unsigned int, unsigned int, unsigned int, unsigned int);
extern void ISR13(); /* GPF exception */
void CISR13(unsigned int, unsigned int, unsigned int, unsigned int);
extern void ISR14(); /* Page fault exception */
void CISR14(unsigned int, unsigned int, unsigned int, unsigned int);
extern void ISR15(); /* Unknown interrupt exception */
void CISR15(unsigned int, unsigned int, unsigned int);
extern void ISR16(); /* Coprocessor fault exception */
void CISR16(unsigned int, unsigned int, unsigned int);
extern void ISR17(); /* Alignment check exception */
void CISR17(unsigned int, unsigned int, unsigned int);
extern void ISR18(); /* Machine check exception */
void CISR18(unsigned int, unsigned int, unsigned int);
extern void ISRReserved(); /* Reserved IRQ */
void CISRReserved(unsigned int, unsigned int, unsigned int);

/* irqs.c */

extern void irq0(); /* Programmable Interrupt Timer */
void CIRQ0();
extern void irq1(); /* Keyboard */
void CIRQ1();
extern void irq7(); /* IRn not raised high for sufficient time */
void CIRQ7();

#endif