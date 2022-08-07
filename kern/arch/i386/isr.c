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

#include <Lunix/kernel/common.h>
#include <Lunix/kernel/isr.h>
#include <Lunix/kernel/monitor.h>

isr_t interrupt_handlers[256];

void register_interrupt_handler(u8int n, isr_t handler)
{

    interrupt_handlers[n] = handler;

}

// This gets called from our ASM interrupt handler stub.

void isr_handler(registers_t regs)
{

    // This line is important. When the processor extends the 8-bit interrupt number
    // to a 32bit value, it sign-extends, not zero extends. So if the most significant
    // bit (0x80) is set, regs.int_no will be very large (about 0xffffff80).

    u8int int_no = regs.int_no & 0xFF;

    if (interrupt_handlers[int_no] != 0)
    {

        isr_t handler = interrupt_handlers[int_no];
        handler(&regs);

    }
    else
    {

        monitor_write("unhandled interrupt: ");
        monitor_write_hex(int_no);
        monitor_put('\n');

        for(;;);

    }

}

// This gets called from our ASM interrupt handler stub.

void irq_handler(registers_t regs)
{

    // Send an EOI (end of interrupt) signal to the PICs.
    // If this interrupt involved the slave.

    if (regs.int_no >= 40)
    {

        // Send reset signal to slave.

        outb(0xA0, 0x20);

    }

    // Send reset signal to master. (As well as slave, if necessary).

    outb(0x20, 0x20);

    if (interrupt_handlers[regs.int_no] != 0)
    {

        isr_t handler = interrupt_handlers[regs.int_no];
        handler(&regs);

    }

}
