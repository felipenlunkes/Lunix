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

#include <Lunix/kernel/monitor.h>
#include <Lunix/console.h>
#include <Lunix/kernel/descriptor_tables.h>
#include <Lunix/kernel/timer.h>
#include <Lunix/kernel/paging.h>
#include <Lunix/kernel/multiboot.h>
#include <Lunix/kernel/fs.h>
#include <Lunix/kernel/initrd.h>
#include <Lunix/kernel/task.h>
#include <Lunix/kernel/syscall.h>
#include <Lunix/kernel/version.h>

extern u32int placement_address;
u32int initial_esp;

int main(struct multiboot *mboot_ptr, u32int initial_stack)
{

    initial_esp = initial_stack;

    init_descriptor_tables();
    
    monitor_clear();

    monitor_write("Lunix kernel version ");
    monitor_write(LUNIX_VERSION);
    monitor_write("\nCopyright (c) 2022 Felipe Miguel Nery Lunkes\n");

    // Initialise the PIT to 100Hz

    enable();

    init_timer(50);

    // Find the location of our initial ramdisk.
    
    ASSERT(mboot_ptr->mods_count > 0);

    u32int initrd_location = *((u32int*)mboot_ptr->mods_addr);
    u32int initrd_end = *(u32int*)(mboot_ptr->mods_addr+4);

    // Don't trample our module with placement accesses, please!

    placement_address = initrd_end;

    initialise_devices();

    // Start paging.

    initialise_paging();

    // Start multitasking.

    initialise_tasking();

    // Initialise the initial ramdisk, and set it as the filesystem root.

    fs_root = initialise_initrd(initrd_location);

    initialise_syscalls();

    disable(); // The follow part is not reentrant!
    
    ls_initrd();

    LXmonitor();

    switch_to_user_mode();

    syscall_monitor_write("\n\nHey, I'm here!\n");

    return 0;

}
