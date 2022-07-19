#include "../libc/string.h"
#include <Lunix/console.h>

void init_COM1(){

    kprint("\nInitializing serial ports (com1)...");

    extern initCOM1();

}