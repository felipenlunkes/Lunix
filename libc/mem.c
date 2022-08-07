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

#include <mem.h>
#include <Lunix/kernel/common.h>

void memory_copy(uint8_t *source, uint8_t *dest, int nbytes) {

    int i;

    for (i = 0; i < nbytes; i++) {

        *(dest + i) = *(source + i);

    }
}

void memory_set(uint8_t *dest, uint8_t val, uint32_t len) {

    uint8_t *temp = (uint8_t *)dest;

    for ( ; len != 0; len--) *temp++ = val;

}


unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count)
{
	unsigned short *ret = (unsigned short*) dest;
	while(count-- != 0)
	{

		*dest++ = val;

	}

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