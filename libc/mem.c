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

/* This should be computed at link time, but a hardcoded
 * value is fine for now. Remember that our kernel starts
 * at 0x1000 as defined on the Makefile */

uint32_t free_mem_addr = 0x10000;

/* Implementation is just a pointer to some free memory which
 * keeps growing */

uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr) {

    /* Pages are aligned to 4K, or 0x1000 */

    if (align == 1 && (free_mem_addr & 0xFFFFF000)) {

        free_mem_addr &= 0xFFFFF000;
        free_mem_addr += 0x1000;

    }

    /* Save also the physical address */

    if (phys_addr) *phys_addr = free_mem_addr;

    uint32_t ret = free_mem_addr;

    free_mem_addr += size; /* Remember to increment the pointer */

    return ret;
    
}
