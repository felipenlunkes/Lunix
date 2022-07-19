#include <Lunix/console.h>
#include <Lunix/kernel/kernel.h>
#include <stdint.h>

void panic(char *message){

    kprint("\n[Lunix kernel] Panic: ");
    kprint(message);

    asm volatile("hlt");

}