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

#include <Lunix/console.h>
#include <string.h>
#include <mem.h>
#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/common.h>
#include <Lunix/kernel/thread.h>
#include <Lunix/ports.h>
#include <stdint.h>

extern setCS();

void halt(void){

  asm ("hlt");
    
}

void disable(void){

  asm ("cli");

}

void enable(void){

  asm volatile ("sti");

}

void nop(void){

  asm volatile ("nop");

}

void outportb(unsigned short port, unsigned char val)
{

	asm volatile("outb %0,%1"::"a"(val), "Nd"(port));

}

void outportw(unsigned short port, unsigned short val)
{

	asm volatile("outw %0,%1"::"a"(val), "Nd" (port));

}

unsigned char inportb(unsigned short port)
{

	unsigned char ret;

	asm volatile("inb %1,%0":"=a"(ret):"Nd"(port));

	return ret;

}

// Write a byte out to the specified port.

void outb(u16int port, u8int value)
{

    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));

}

u8int inb(u16int port)
{

    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));

    return ret;

}

u16int inw(u16int port)
{

    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));

    return ret;

}
