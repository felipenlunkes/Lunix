#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "kernel.h"
#include "version.h"
#include <stdint.h>

lunix_shell();

void kernel_main() {

    kprint("\nWelcome to Lunix kernel!\n");
    kprint("Lunix kernel version ");
    kprint(LUNIX_VERSION);
    kprint("\n");

    isr_install();
    irq_install();
    init_COM1();
    init_Parallel();

    //asm("int $2");
    //asm("int $3");

    init_shell();

}
