#ifndef KERNEL_H
#define KERNEL_H

#include <Lunix/kernel/common.h>

void panic(const char *message, const char *file, u32int line);
void panic_assert(const char *file, u32int line, const char *desc);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

#endif
