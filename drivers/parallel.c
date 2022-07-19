#include "../libc/string.h"
#include "../drivers/screen.h"

void init_Parallel(){

    kprint("\nInitializing parallel ports (imp0)...");

    extern initParallel();

}