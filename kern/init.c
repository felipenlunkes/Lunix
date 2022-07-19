#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/isr.h>
#include <Lunix/kernel/thread.h>
#include <stdint.h>

void init_LXmonitor();
 
static void kernel_thread() {
    
    kprint("\nkernel_thread: PID 0 started");

    kprint("\nStarting PID 1 (LX monitor)...");
     
    kexec(init_LXmonitor, 1);
 
    yield();

}

void initTasking(){
 
  kprint("\nInitializing threading...");
 
  kexec(kernel_thread, 0); // PID 0 for the kernel_thread
 
  kprint("\nCreating the first thread...");
 
  yield();
 
}
