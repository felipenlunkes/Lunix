#ifndef __TASK_H__
#define __TASK_H__
 
#include <stdint.h>
 
extern void initTasking();
 
typedef struct {

    uint32_t eax, ebx, ecx, edx, esi, edi, esp, ebp, eip, eflags, cr3;
    
} Registers;
 
typedef struct Task {

    Registers regs;
    struct Task *next;

} Task;
 
extern void initTasking();
extern void kexec(void(*)());
extern void createTask(Task*, void(*)(), uint32_t, uint32_t*);
 
extern void yield(); // Switch task frontend
extern void switchTask(Registers *old, Registers *new); // The function which actually switches
 
#endif /* __TASK_H__ */
