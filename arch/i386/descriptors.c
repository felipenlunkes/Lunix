/*
Copyright (c) 2022, Felipe Miguel Nery Lunkes
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
#include <Lunix/kernel/descriptors.h>

void setupIDT()
{

    kprint("\nInitializing the IDT...");

	IDTPtr.limit = (sizeof(struct IDTEntry) * 256) - 1;
	IDTPtr.base = (unsigned int) &IDT;

	memset((unsigned char *)&IDT, 0x00, sizeof(struct IDTEntry) * 256);
	struct IDTEncode *entry;
	
	entry->base = (unsigned) ISR0;
	entry->segment = 0x08;
	entry->flags = 0x8E;
	EncodeIDTEntry(0, entry);

	entry->base = (unsigned) ISR1;
	EncodeIDTEntry(1, entry);

	entry->base = (unsigned) ISR2;
	EncodeIDTEntry(2, entry);

	entry->base = (unsigned) ISR3;
	EncodeIDTEntry(3, entry);

	entry->base = (unsigned) ISR4;
	EncodeIDTEntry(4, entry);

	entry->base = (unsigned) ISR5;
	EncodeIDTEntry(5, entry);

	entry->base = (unsigned) ISR6;
	EncodeIDTEntry(6, entry);

	entry->base = (unsigned) ISR7;
	EncodeIDTEntry(7, entry);

	entry->base = (unsigned) ISR8;
	EncodeIDTEntry(8, entry);

	entry->base = (unsigned) ISR9;
	EncodeIDTEntry(9, entry);

	entry->base = (unsigned) ISR10;
	EncodeIDTEntry(10, entry);

	entry->base = (unsigned) ISR11;
	EncodeIDTEntry(11, entry);

	entry->base = (unsigned) ISR12;
	EncodeIDTEntry(12, entry);

	entry->base = (unsigned) ISR13;
	EncodeIDTEntry(13, entry);

	entry->base = (unsigned) ISR14;
	EncodeIDTEntry(14, entry);

	entry->base = (unsigned) ISRReserved;
	EncodeIDTEntry(15, entry);

	entry->base = (unsigned) ISR16;
	EncodeIDTEntry(16, entry);

	entry->base = (unsigned) ISR17;
	EncodeIDTEntry(17, entry);

	entry->base = (unsigned) ISR18;
	EncodeIDTEntry(18, entry);

	entry->base = (unsigned) ISRReserved;
	for(int index = 19; index < 32; index++)
	{

		EncodeIDTEntry(index, entry);

	}

	entry->base = (unsigned) irq0;
	EncodeIDTEntry(32, entry);

	entry->base = (unsigned) irq1;
	EncodeIDTEntry(33, entry);

	entry->base = (unsigned) irq7;
	EncodeIDTEntry(39, entry);

	loadIDT();

}