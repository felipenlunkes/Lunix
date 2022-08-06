/*
Copyright (c) 2022, Felipe Miguel Nery Lunkes
All rights reserved.

Portions Copyright (c) 2018, Carlos Fenollosa
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <Lunix/io.h>
#include <Lunix/console.h>
#include <Lunix/kernel/isr.h>
#include <Lunix/keyboard.h>
#include <string.h>
#include <Lunix/timer.h>
#include <Lunix/ports.h>


int ticks = 0; /* Keeps count of the ticks of the PIT */

void remapPIC()
{
	
	kprint("\nRemaping PIC...");

	#define PIC1 0x20 /* The first PIC in the chain and */
	#define PIC2 0xA0 /* the second. Data is PIC+1. */
	
	#define ICW1 0x11

	/* Initialisation control word 1, explanation:
		0001 0001
		Bits 5-7:
			Are set to 0 in 8086/8088 mode
		Bit 4:
			ICW is indicated by a control word with D4 set.
		Bits 0-3:
			Bit 3 - LTIM - If set then PIC operates in level
				interrupt mode, else edge detect).
			Bit 2 - ADI - CALL address interval, if set then
				4, else 0.
			Bit 1 - SNGL - If set, only one PIC used, so no ICW3
				is issued.
			Bit 0 - IC4 - If set, PIC will read ICW4.
	*/

	/* Initialisation control word 2, explanation:
		This is the vector offsets for the indices into the IDT for each
		PIC.
	*/
	
	/* Initialisation control word 3, explanation:
		0000 0100 for master (PIC1)
		0000 0010 for slave (PIC2)
		For PIC1, the bit set is the IR input with a slave.
		For PIC2, the least three bits indicate the IR input to which
			it is slave (or alternatively, the master IR input 
	*/

	#define ICW4 0x01

	/* Initialisation control word 4, explanation:
		0000 0001
		Bits 5-7:
			Are set to 0 in 8086/8088 mode
		Bit 4-0:
			Bit 4 - SFNM - Set for special fully nested mode, else
				not special fully nested mode
			Bit 3 - BUF - Set for buffered mode
			Bit 2 - M/S - If BUF set then set for master, else slave
			Bit 1 - AEOI - Set for auto End of Interrupt
			Bit 0 - muPM - Set for 8086/8088 mode, else MCS-80/85
	*/


			
	unsigned char IRQmaskPIC1 = 0x00;
	unsigned char IRQmaskPIC2 = 0x00;

	/* Store the IRQ masks for each PIC */

	IRQmaskPIC1 = inportb(PIC1 + 1);
	IRQmaskPIC2 = inportb(PIC2 + 1);

	/* Send ICW1 to each PIC */

	outportb(PIC1,ICW1);
	wait();
	outportb(PIC2,ICW1);
	wait();

	/* Send ICW2 to each PIC */

	outportb(PIC1 + 1, 0x20);
	wait();
	outportb(PIC2 + 1, 0x28);
	wait();

	/* Send ICW3 to each PIC */

	outportb(PIC1 + 1, 0x04);
	wait();
	outportb(PIC2 + 1, 0x02);
	wait();

	/* Send ICW4 to each PIC */

	outportb(PIC1 + 1, ICW4);
	wait();
	outportb(PIC2 + 1, ICW4);
	wait();

	/* Restore interrupt masks */

	outportb(PIC1 + 1, IRQmaskPIC1);
	outportb(PIC2 + 1, IRQmaskPIC2);

	outportb(0x21, 0xFD);
	outportb(0xA1, 0xFF);

	kprint(" [done]");

}

void pic_disable(void){

	kprint("\nTurning off PIC...");

	outportb(0x21, 0xFF);
	outportb(0xA1, 0xFF);

	kprint(" [done]");

}

void CISR0(unsigned int eip, unsigned int cs, unsigned int eflags)
{

	/* Divide by zero exception */

	puts("\nEXCEPTION: Divide by zero");
	
	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR1(unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Debug exception */

	puts("\nEXCEPTION: Debug");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR2(unsigned int eip, unsigned int cs, unsigned int eflags)
{

	/* NMI exception */
	
	puts("\nEXCEPTION: Non maskable interrupt");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR3(unsigned int eip, unsigned int cs, unsigned int eflags)
{

	/* Breakpoint exception */
	
	puts("\nEXCEPTION: Breakpoint");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR4(unsigned int eip, unsigned int cs, unsigned int eflags)
{

	/* Overflow exception */
	
	puts("\nEXCEPTION: Overflow");
	
	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR5(unsigned int eip, unsigned int cs, unsigned int eflags)
{

	/* Out of bounds exception */
	
	puts("\nEXCEPTION: Out of bounds");
	
	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR6(unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Invalid opcode exception */
	
	puts("\nEXCEPTION: Invalid opcode");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR7(unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* No coprocessor exception */
	
	puts("\nEXCEPTION: No coprocessor");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR8(unsigned int errcode, unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Double fault exception */
	
	puts("\nEXCEPTION: Double fault");
	
	char buf[10];

	puts("\n Error: 0x");
	puts(itoa(errcode, buf, 16));
	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR9(unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Coprocessor segment overrun exception */
	
	puts("\nEXCEPTION: Coprocessor segment overrun exception");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR10(unsigned int errcode, unsigned int eip, unsigned int cs, unsigned int eflags)
{

	/* Bad TSS exception */
	
	puts("\nEXCEPTION: Bad TSS");

	char buf[10];

	puts("\n Error: 0x");
	puts(itoa(errcode, buf, 16));
	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR11(unsigned int errcode, unsigned int eip, unsigned int cs, unsigned int eflags)
{

	/* Segment not present exception */
	
	puts("\nEXCEPTION: Segment not present");

	char buf[10];

	puts("\n Error: 0x");
	puts(itoa(errcode, buf, 16));
	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR12(unsigned int errcode, unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Stack fault exception */
	
	puts("\nEXCEPTION: Stack fault");

	char buf[10];

	puts("\n Error: 0x");
	puts(itoa(errcode, buf, 16));
	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR13(unsigned int errcode, unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* GPF exception */

	puts("\nEXCEPTION: General protection fault");

	char buf[10];

	puts("\n Error: 0x");
	puts(itoa(errcode, buf, 16));
	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR14(unsigned int errcode, unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Page fault exception */

	puts("\nEXCEPTION: Page fault");

	char buf[10];

	puts("\n Error: 0x");
	puts(itoa(errcode, buf, 16));
	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR15(unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Unknown interrupt exception */

	puts("\nEXCEPTION: Unknown interrupt");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR16(unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Coprocessor fault exception */

	puts("\nEXCEPTION: Coprocessor fault");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR17(unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Alignment check exception */

	puts("\nEXCEPTION: Alignment check");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISR18(unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Machine check exception */

	puts("\nEXCEPTION: Machine check");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void CISRReserved(unsigned int eip, unsigned int cs, unsigned int eflags)
{
	/* Reserved IRQ */

	puts("\nEXCEPTION: Reserved IRQ");

	char buf[10];

	puts("\n EIP: 0x");
	puts(itoa(eip, buf, 16));
	puts("\n CS: 0x");
	puts(itoa(cs, buf, 16));
	puts("\n EFLAGS: 0x");
	puts(itoa(eflags, buf, 16));

	for(;;);

}

void wait()
{

	for(int i = 0; i < 6; i++) outportb(0x80, 0x00);

}

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

/* Segment selectors */

#define KERNEL_CS 0x08

/* How every interrupt gate (handler) is defined */

typedef struct {

    uint16_t low_offset; /* Lower 16 bits of handler function address */
    uint16_t sel; /* Kernel segment selector */
    uint8_t always0;

    /* First byte
     * Bit 7: "Interrupt is present"
     * Bits 6-5: Privilege level of caller (0=kernel..3=user)
     * Bit 4: Set to 0 for interrupt gates
     * Bits 3-0: bits 1110 = decimal 14 = "32 bit interrupt gate" */

    uint8_t flags; 
    uint16_t high_offset; /* Higher 16 bits of handler function address */

} __attribute__((packed)) idt_gate_t ;

/* A pointer to the array of interrupt handlers.
 * Assembly instruction 'lidt' will read it */

void set_idt_gate(int n, uint32_t handler);

extern void set_idt();

typedef struct {

    uint16_t limit;
    uint32_t base;

} __attribute__((packed)) idt_register_t;

#endif

isr_t interrupt_handlers[256];

/* Can't do this with a loop because we need the address
 * of the function names */

void isr_install() {

    kprint("\nInstalling the interrupt routines...");
    
    set_idt_gate(0, (uint32_t)isr0);
    set_idt_gate(1, (uint32_t)isr1);
    set_idt_gate(2, (uint32_t)isr2);
    set_idt_gate(3, (uint32_t)isr3);
    set_idt_gate(4, (uint32_t)isr4);
    set_idt_gate(5, (uint32_t)isr5);
    set_idt_gate(6, (uint32_t)isr6);
    set_idt_gate(7, (uint32_t)isr7);
    set_idt_gate(8, (uint32_t)isr8);
    set_idt_gate(9, (uint32_t)isr9);
    set_idt_gate(10, (uint32_t)isr10);
    set_idt_gate(11, (uint32_t)isr11);
    set_idt_gate(12, (uint32_t)isr12);
    set_idt_gate(13, (uint32_t)isr13);
    set_idt_gate(14, (uint32_t)isr14);
    set_idt_gate(15, (uint32_t)isr15);
    set_idt_gate(16, (uint32_t)isr16);
    set_idt_gate(17, (uint32_t)isr17);
    set_idt_gate(18, (uint32_t)isr18);
    set_idt_gate(19, (uint32_t)isr19);
    set_idt_gate(20, (uint32_t)isr20);
    set_idt_gate(21, (uint32_t)isr21);
    set_idt_gate(22, (uint32_t)isr22);
    set_idt_gate(23, (uint32_t)isr23);
    set_idt_gate(24, (uint32_t)isr24);
    set_idt_gate(25, (uint32_t)isr25);
    set_idt_gate(26, (uint32_t)isr26);
    set_idt_gate(27, (uint32_t)isr27);
    set_idt_gate(28, (uint32_t)isr28);
    set_idt_gate(29, (uint32_t)isr29);
    set_idt_gate(30, (uint32_t)isr30);
    set_idt_gate(31, (uint32_t)isr31);

    // Install the IRQs

    set_idt_gate(32, (uint32_t)irq0);
    set_idt_gate(33, (uint32_t)irq1);
    set_idt_gate(34, (uint32_t)irq2);
    set_idt_gate(35, (uint32_t)irq3);
    set_idt_gate(36, (uint32_t)irq4);
    set_idt_gate(37, (uint32_t)irq5);
    set_idt_gate(38, (uint32_t)irq6);
    set_idt_gate(39, (uint32_t)irq7);
    set_idt_gate(40, (uint32_t)irq8);
    set_idt_gate(41, (uint32_t)irq9);
    set_idt_gate(42, (uint32_t)irq10);
    set_idt_gate(43, (uint32_t)irq11);
    set_idt_gate(44, (uint32_t)irq12);
    set_idt_gate(45, (uint32_t)irq13);
    set_idt_gate(46, (uint32_t)irq14);
    set_idt_gate(47, (uint32_t)irq15);

    set_idt(); // Load with ASM

	kprint(" [done]");

}

/* To print the message which defines every exception */

char *exception_messages[] = {

    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"

};

void isr_handler(registers_t *r) {

    kprint("\nKern: received interrupt ");

    char s[3];

    int_to_ascii(r->int_no, s);
    
    kprint(s);
    kprint("\n");
    kprint(exception_messages[r->int_no]);
    kprint("\n");

}

void register_interrupt_handler(uint8_t n, isr_t handler) {

    interrupt_handlers[n] = handler;

}

void irq_handler(registers_t *r) {

    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */

    if (r->int_no >= 40) port_byte_out(0xA0, 0x20); /* slave */

    port_byte_out(0x20, 0x20); /* master */

    /* Handle the interrupt in a more modular way */

    if (interrupt_handlers[r->int_no] != 0) {

        isr_t handler = interrupt_handlers[r->int_no];

        handler(r);

    }
}

void irq_install() {

    /* IRQ0: timer */

     init_timer(50);

    /* IRQ1: keyboard */

    init_keyboard();

	kprint("\nKernel services started successfully...");

    kprint(" [done]");

}
