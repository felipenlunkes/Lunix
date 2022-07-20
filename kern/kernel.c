#include <Lunix/kernel/isr.h>
#include <Lunix/console.h>
#include <Lunix/io.h>
#include "../libc/string.h"
#include "../libc/mem.h"
#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/version.h>
#include <stdint.h>

void kern_main() {

    kprint("\nLunix kernel version ");
    kprint(LUNIX_VERSION);
    kprint("\nCopyright (c) 2022 Felipe Miguel Nery Lunkes\n");

    config_x86();
    isr_install();
    irq_install();
    init_COM1();
    init_Parallel();

    // asm("int $2");
    // asm("int $3");

    initTasking();

}

