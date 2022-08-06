#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>

#define low_16(address) (uint16_t)((address) & 0xFFFF)
#define high_16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

typedef unsigned long size_t;

#endif
