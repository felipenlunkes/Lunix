#include <Lunix/console.h>
#include "../libc/string.h"
#include "../libc/mem.h"
#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/thread.h>
#include <stdint.h>

void init_LXmonitor(){

int setUser=0;

    kprint("\n\nWelcome to the Lunix kernel monitor\n");

    LXset_user(1, "root", 777, 1);

    kprint("\n");

    lunix_monitor();

    yield();

}

void lunix_monitor(char *input) {

    if (strcmp(input, "PANIC") == 0) {

        panic("the user forced a kernel panic. Reboot\n");

    } else if (strcmp(input, "KMALLOC") == 0) {

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

    else if (strcmp(input, "HELP") == 0) {

        kprint("\nThe kernel monitor allows you to:\n");
        kprint("\n1: Test kernel functions");
        kprint("\n2: Test the hardware response");
        kprint("\nA lot is to come!\n\n");
    
    }

    else if (strcmp(input, "REBOOT") == 0) {

        LXreboot();
    
    }

    else if (strcmp(input, "USER") == 0) {

        int userID=LXget_userID(1);

        kprint("\nUser ID: ");
        kprint(userID);
    }

    else if (strcmp(input, "CLEAR") == 0) {

        clear_screen();
        
    }

    kprint("> ");

}
