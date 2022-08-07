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

// Copy len bytes from src to dest.

void memcpy(u8int *dest, const u8int *src, u32int len)
{

    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;

    for(; len != 0; len--) *dp++ = *sp++;

}

// Write len copies of val into dest.

void memset(u8int *dest, u8int val, u32int len)
{

    u8int *temp = (u8int *)dest;

    for ( ; len != 0; len--) *temp++ = val;

}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.

int strcmp(char *str1, char *str2)
{

      int i = 0;
      int failed = 0;

      while(str1[i] != '\0' && str2[i] != '\0')
      {
        
          if(str1[i] != str2[i])
          {

              failed = 1;

              break;

          }

          i++;

      }

      // why did the loop exit?

      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;

}

// Copy the NULL-terminated string src into dest, and
// return dest.

char *strcpy(char *dest, const char *src)
{

    do
    {

      *dest++ = *src++;

    }

    while (*src != 0);

}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.

char *strcat(char *dest, const char *src)
{

    while (*dest != 0)
    {

        *dest = *dest++;

    }

    do
    {

        *dest++ = *src++;

    }

    while (*src != 0);

    return dest;

}

int strlen(char *src)
{

    int i = 0;

    while (*src++)
        i++;

    return i;

}

extern void panic(const char *message, const char *file, u32int line)
{

    // We encountered a massive problem and have to stop.

    asm volatile("cli"); // Disable interrupts.

    monitor_write("\nPANIC: (");
    monitor_write(message);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_write("\n");

    // Halt by going into an infinite loop.

    for(;;);

}

extern void panic_assert(const char *file, u32int line, const char *desc)
{

    // An assertion failed, and we have to panic.

    asm volatile("cli"); // Disable interrupts.

    monitor_write("ASSERTION-FAILED(");
    monitor_write(desc);
    monitor_write(") at ");
    monitor_write(file);
    monitor_write(":");
    monitor_write_dec(line);
    monitor_write("\n");

    // Halt by going into an infinite loop.

    for(;;);
    
}
