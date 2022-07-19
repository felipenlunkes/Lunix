#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/isr.h>
#include <stdint.h>

#include "thread.h"
 
static Task *runningTask;
static Task mainTask;
static Task otherTask;

void init_shell();
 
static void kernel_thread() {
    
    kprint("\nkernel_thread: PID 0 started");

    init_LXmonitor();

}
 
void initTasking() {


    // Get EFLAGS and CR3

    kprint("\nInitializing threading...");

    asm volatile("movl %%cr3, %%eax; movl %%eax, %0;":"=m"(mainTask.regs.cr3)::"%eax");
    asm volatile("pushfl; movl (%%esp), %%eax; movl %%eax, %0; popfl;":"=m"(mainTask.regs.eflags)::"%eax");
 
    createTask(&otherTask, kernel_thread, mainTask.regs.eflags, (uint32_t*)mainTask.regs.cr3);

    mainTask.next = &otherTask;
    otherTask.next = &mainTask;
 
    runningTask = &mainTask;

    yield();

}
 
void createTask(Task *task, void (*main)(), uint32_t flags, uint32_t *pagedir) {

    kprint("\nCreating the first thread...");

    uint32_t phys_addr;

    task->regs.eax = 0;
    task->regs.ebx = 0;
    task->regs.ecx = 0;
    task->regs.edx = 0;
    task->regs.esi = 0;
    task->regs.edi = 0;
    task->regs.eflags = flags;
    task->regs.eip = (uint32_t) main;
    task->regs.cr3 = (uint32_t) pagedir;
    task->regs.esp = kmalloc(1000, 1, &phys_addr);
    task->next = 0;

}
 
void yield() {

    Task *last = runningTask;
    runningTask = runningTask->next;

    switchTask(&last->regs, &runningTask->regs);

}