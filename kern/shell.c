#include <Lunix/console.h>
#include "../libc/string.h"
#include "../libc/mem.h"
#include <Lunix/kernel/kernel.h>
#include <stdint.h>

void init_shell(){

    kprint("\n\nWelcome to the Lunix kernel shell\n");

    lunix_shell();

}

void lunix_shell(char *input) {

    if (strcmp(input, "PANIC") == 0) {

        panic("the user forced a kernel panic. Reboot\n");

    } else if (strcmp(input, "MALLOC") == 0) {

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
