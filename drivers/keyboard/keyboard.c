/*
Copyright (c) 2018, Carlos Fenollosa
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

#include <Lunix/keyboard.h>
#include <Lunix/ports.h>
#include <Lunix/kernel/isr.h>
#include <Lunix/console.h>
#include "../libc/string.h"
#include "../libc/function.h"
#include <Lunix/kernel/kernel.h>
#include <stdint.h>
#include "keyboard.h"

#define BACKSPACE 0x0E
#define ENTER 0x1C

char letter;

static char key_buffer[256];

#define SC_MAX 57

char resolvChar(uint8_t byte){

    return sc_ascii[byte];

}

uint8_t scanf(void) {

    unsigned char scan;

    static uint8_t letter = 0;

    uint8_t keychar = port_byte_in(0x60); 

    scan = keychar & 0x80;
    keychar = keychar & 0x7f;

    if (scan)
    {

        return letter = 0;

    }

    else if (keychar != letter)
    {

        return letter = keychar;

    }

    else
    {

        return 0;

    }

}

static void keyboard_callback(registers_t *regs) {

    /* The PIC leaves us the scancode in port 0x60 */

    uint8_t scancode = port_byte_in(0x60);
    
    if (scancode > SC_MAX) return;

    if (scancode == BACKSPACE) {

        backspace(key_buffer);

        kprint_backspace();

    } else if (scancode == ENTER) {

        kprint("\n");

        //LXmonitor(key_buffer); /* kernel-controlled function */

        key_buffer[0] = '\0';

    } else {

        letter = sc_ascii[(int)scancode];

        /* Remember that kprint only accepts char[] */

        char str[2] = {letter, '\0'};

        //append(key_buffer, letter);

        //kprint(str);

    }

    UNUSED(regs);

}

void init_keyboard() {

   register_interrupt_handler(IRQ1, keyboard_callback); 
   
}
