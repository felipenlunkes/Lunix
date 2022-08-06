#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stddef.h>

void memory_copy(uint8_t *source, uint8_t *dest, int nbytes);
void memory_set(uint8_t *dest, uint8_t val, uint32_t len);
unsigned char *memset(unsigned char *dest, unsigned char val, size_t count);
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);

/* At this stage there is no 'free' implemented. */

uint32_t kmalloc(size_t size, int align, uint32_t *phys_addr);

#endif
