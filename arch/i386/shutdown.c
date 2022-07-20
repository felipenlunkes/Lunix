#include <Lunix/console.h>
#include "../libc/string.h"
#include "../libc/mem.h"
#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/thread.h>
#include <Lunix/ports.h>
#include <stdint.h>

void LXshutdown(void){

    port_word_out(0xB004, 0x2000); // Bochs
    port_word_out(0x604, 0x2000);  // Qemu
    port_word_out(0x4004, 0x3400); // VirtualBox

// TODO: real hardware

}