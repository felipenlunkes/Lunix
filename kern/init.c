#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/isr.h>
#include <Lunix/kernel/thread.h>
#include <stdint.h>

void init_LXmonitor();
 
static void kernel_thread() {
    
    kprint("\nkernel_thread: PID 0 started");

    init_LXmonitor();

}

void initTasking(){
 
  kprint("\nInitializing threading...");
 
  exec(kernel_thread);
 
  kprint("\nCreating the first thread...");
 
  yield();
 
}
