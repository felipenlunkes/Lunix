/*
Copyright (c) 2022, Felipe Miguel Nery Lunkes
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef TASK_H
#define TASK_H

#include "common.h"
#include "paging.h"

#define KERNEL_STACK_SIZE 2048       // Use a 2kb kernel stack.

// This structure defines a 'task' - a process.

typedef struct task
{

    int id;                // Process ID.
    u32int esp, ebp;       // Stack and base pointers.
    u32int eip;            // Instruction pointer.
    page_directory_t *page_directory; // Page directory.
    u32int kernel_stack;   // Kernel stack location.
    struct task *next;     // The next task in a linked list.

} task_t;

// Initialises the tasking system.

void initialise_tasking();

// Called by the timer hook, this changes the running process.

void task_switch();

// Forks the current process, spawning a new one with a different
// memory space.

int fork();

// Causes the current process' stack to be forcibly moved to a new location.

void move_stack(void *new_stack_start, u32int size);

// Returns the pid of the current process.

int getpid();

#endif
