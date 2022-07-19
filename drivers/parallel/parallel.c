#include "../libc/string.h"
#include <Lunix/console.h>

void init_Parallel(){

    kprint("\nInitializing parallel ports (imp0)...");

    extern initParallel();

}