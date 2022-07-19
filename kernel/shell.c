#include "../cpu/isr.h"
#include "../drivers/screen.h"
#include "../libc/string.h"
#include "../libc/mem.h"
#include "kernel.h"
#include "version.h"
#include <stdint.h>

void init_shell(){

    kprint("\n\nWelcome to the Lunix kernel shell\n");

    lunix_shell();

}

void lunix_shell(char *input) {

    if (strcmp(input, "END") == 0) {

        kprint("Stopping the CPU. Bye!\n");

        asm volatile("hlt");

    } else if (strcmp(input, "PAGE") == 0) {

        uint32_t phys_addr;
        uint32_t page = kmalloc(1000, 1, &phys_addr);
        char page_str[16] = "";
        
        hex_to_ascii(page, page_str);
        
        char phys_str[16] = "";
       
        hex_to_ascii(phys_addr, phys_str);
       
        kprint("Page: ");
        kprint(page_str);
        kprint(", physical address: ");
        kprint(phys_str);
        kprint("\n");
    }

    kprint("> ");

}
