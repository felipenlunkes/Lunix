#include <Lunix/console.h>
#include "../libc/string.h"
#include "../libc/mem.h"
#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/thread.h>
#include <Lunix/ports.h>
#include <stdint.h>

void LXreboot(void){

    kprint("\nRebootinh the computer...");

    doReboot();

}

void doReboot(void){

uint8_t good = 0x02;

    while (good & 0x02){

        good = port_byte_in(0x64);
    
    }

    port_byte_out(0x64, 0xFE);

    halt();

}
