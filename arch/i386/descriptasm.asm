;; Copyright (c) 2018, Carlos Fenollosa
;; Copyright (c) 2022, Felipe Miguel Nery Lunkes
;; All rights reserved.

;; Redistribution and use in source and binary forms, with or without
;; modification, are permitted provided that the following conditions are met:
;;
;; * Redistributions of source code must retain the above copyright notice, this
;;  list of conditions and the following disclaimer.
;;
;; * Redistributions in binary form must reproduce the above copyright notice,
;;  this list of conditions and the following disclaimer in the documentation
;;  and/or other materials provided with the distribution.
;;
;; * Neither the name of the copyright holder nor the names of its
;;  contributors may be used to endorse or promote products derived from
;;  this software without specific prior written permission.
;; 
;; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
;; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
;; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
;; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
;; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
;; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
;; SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
;; CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
;; OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
;; OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

global reloadGDT			; reloadGDT function visible to linker
global loadIDT				; loadIDT function visible to linker

extern IDTPtr				; Our pointer to the IDT

reloadGDT:

	lgdt	[GDTDesc]	; Load the GDT as our GDTPtr

	jmp	0x08:GDTfin		; CS is GDT[1], far jmp flushes GDT

GDTfin:
	mov	AX,		0x10	; DS is GDT[2]
	mov	DS,		AX
	mov	ES,		AX
	mov	FS,		AX
	mov	GS,		AX
	mov	SS,		AX
	ret				; Return

loadIDT:
	lidt	[IDTPtr]		; Load the IDT as our IDTPtr
	ret				; Return

; External declarations of C IRQ handlers
extern CISR0				; Divide by zero exception
global ISR0
extern CISR1				; Debug exception
global ISR1
extern CISR2				; NMI exception
global ISR2
extern CISR3				; Breakpoint exception
global ISR3
extern CISR4				; Overflow exception
global ISR4
extern CISR5				; Out of bounds exception
global ISR5
extern CISR6				; Invalid opcode exception
global ISR6
extern CISR7				; No coprocessor exception
global ISR7
extern CISR8				; Double fault exception
global ISR8
extern CISR9				; Coprocessor segment overrun exception
global ISR9
extern CISR10				; Bad TSS exception
global ISR10
extern CISR11				; Segment not present exception
global ISR11
extern CISR12				; Stack fault exception
global ISR12
extern CISR13				; GPF exception
global ISR13
extern CISR14				; Page fault exception
global ISR14
extern CISR16				; Coprocessor fault exception
global ISR16
extern CISR17				; Alignment check exception
global ISR17
extern CISR18				; Machine check exception
global ISR18
extern CISRReserved			; Reserved IRQ
global ISRReserved

extern CIRQ0				; Programmable Interval Timer
global IRQ0
extern CIRQ1				; Keyboard
global IRQ1
extern CIRQ7				; IRn not raised for long enough
global IRQ7

ISR0:					; Divide by zero exception
	cli				; Cancel interrupts
	call	CISR0			; Call our C handler
	iret				; Return from interrupt

ISR1:					; Debug exception
	cli				; Cancel interrupts
	call	CISR1			; Call our C handler
	iret				; Return from interrupt

ISR2:					; NMI exception
	cli				; Cancel interrupts
	call	CISR2			; Call our C handler
	iret				; Return from interrupts

ISR3:					; Breakpoint exception
	cli				; Cancel interrupts
	call	CISR3			; Call our C handler
	iret				; Return from interrupt

ISR4:					; Overflow exception
	cli				; Cancel interrupts
	call	CISR4			; Call our C handler
	iret				; Return from interrupt

ISR5:					; Out of bounds exception
	cli				; Cancel interrupts
	call	CISR5			; Call our C handler
	iret				; Return from interrupt

ISR6:					; Invalid opcode exception
	cli				; Cancel interrupts
	call	CISR6			; Call our C handler
	iret				; Return from interrupt

ISR7:					; No coprocessor exception
	cli				; Cancel interrupts
	call	CISR7			; Call our C handler
	iret				; Return from interrupt

ISR8:					; Double fault exception
	cli				; Cancel interrupts
	call	CISR8			; Call our C handler
	iret				; Return from interrupt

ISR9:					; Coprocessor segment overrun exception
	cli				; Cancel interrupts
	call	CISR9			; Call our C handler
	iret				; Return from interrupt

ISR10:					; Bad TSS exception
	cli				; Cancel interrupts
	call	CISR10			; Call our C handler
	iret				; Return from interrupt

ISR11:					; Segment not present exception
	cli				; Cancel interrupts
	call	CISR11			; Call our C handler
	iret				; Return from interrupt

ISR12:					; Stack fault exception
	cli				; Cancel interrupts
	call	CISR12			; Call our C handler
	iret				; Return from interrupt

ISR13:					; GPF exception
	cli				; Cancel interrupts
	call	CISR13			; Call our C handler
	iret				; Return from interrupt

ISR14:					; Page fault exception
	cli				; Cancel interrupts
	call	CISR14			; Call our C handler
	iret				; Return from interrupt

ISR16:					; Coprocessor fault exception
	cli				; Cancel interrupts
	call	CISR16			; Call our C handler
	iret				; Return from interrupt

ISR17:					; Alignment check exception
	cli				; Cancel interrupts
	call	CISR17			; Call our C handler
	iret				; Return from interrupt

ISR18:					; Machine check exception
	cli				; Cancel interrupts
	call	CISR18			; Call our C handler
	iret				; Return from interrupt

ISRReserved:				; Reserved IRQ
	cli				; Cancel interrupts
	call	CISRReserved		; Call our C handler
	iret				; Return from interrupt

IRQ0:					; Programmable Interval Timer
	call	CIRQ0			; Call our C handler
	iret				; Return from interrupt

IRQ1:					; Keyboard
	call	CIRQ1			; Call our C handler
	iret				; Return from interrupt

IRQ7:	; IRn is no longer raised high when first INTA-bar is received
	; from the CPU.
	call	CIRQ7			; Call our C handler
	iret				; Return from interrupt

GDTDesc:				; GDT Pointer
	dw		SIZEOFGDT	; GDT limit
	dd		GDT		; GDT base

GDT:					; Global Descriptor Table
	; NULL descriptor - offset 0x00
	db	0x00			; Lowest byte of limit
	db	0x00			; Second lowest byte of limit
	db	0x00			; Lowest byte of base
	db	0x00			; Second lowest byte of base
	db	0x00			; Third lowest byte of base
	db	0x00			; Byte five
	db	0x00			; Byte six
	db	0x00			; Highest byte of base

	; Ring 0 code descriptor - offset 0x08
	db	0xFF			; Lowest byte of limit
	db	0xFF			; Second lowest byte of limit
	db	0x00			; Lowest byte of base
	db	0x00			; Second lowest byte of base
	db	0x00			; Third lowest byte of base
	db	0x9A			; Byte five
	db	0xCF			; Byte six
	db	0x00			; Highest byte of base
	
	; Ring 0 data descriptor - offset 0x10
	db	0xFF			; Lowest byte of limit
	db	0xFF			; Second lowest byte of limit
	db	0x00			; Lowest byte of base
	db	0x00			; Second lowest byte of base
	db	0x00			; Third lowest byte of base
	db	0x92			; Byte five
	db	0xCF			; Byte six
	db	0x00			; Highest byte of base

	; sizeof(GDT) for GDT pointer
	SIZEOFGDT	equ	$-GDT
	