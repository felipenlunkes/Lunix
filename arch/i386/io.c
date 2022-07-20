#include <Lunix/console.h>
#include "../libc/string.h"
#include "../libc/mem.h"
#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/thread.h>
#include <Lunix/ports.h>
#include <stdint.h>

void halt(void){

    asm ("hlt");
    
}

void disable(void){

    asm ("cli");

}

void enable(void){

    asm ("sti");

}

void nop(void){

    asm ("nop");

}