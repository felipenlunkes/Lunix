#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/thread.h>
#include <stdint.h>

void init_LXmonitor();

static void kernel_thread() {
    
    kprint("\nkernel_thread: PID 0 started");

    yield();

}

void initTasking(){

    kprint("\nInitializing threading...");

    exec(kernel_thread, 0);

    kprint("\nCreating the kernel thread...");

    yield();

    kprint("\nCreating the first thread (PID 1)...");

    exec(init_LXmonitor, 1);

    yield();

}