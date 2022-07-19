#include <Lunix/console.h>
#include <Lunix/keyboard.h>
#include "../libc/string.h"

void config_x86(){

    kprint("\nSetting up the processor...");

    asm volatile ("finit");
    asm volatile ("fwait");

}