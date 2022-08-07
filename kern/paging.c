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

#include <Lunix/kernel/paging.h>
#include <Lunix/kernel/kheap.h>

// The kernel's page directory

page_directory_t *kernel_directory=0;

// The current page directory;

page_directory_t *current_directory=0;

// A bitset of frames - used or free.

u32int *frames;
u32int nframes;

// Defined in kheap.c

extern u32int placement_address;
extern heap_t *kheap;

// Macros used in the bitset algorithms.

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// Static function to set a bit in the frames bitset

static void set_frame(u32int frame_addr)
{

    u32int frame = frame_addr/0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int off = OFFSET_FROM_BIT(frame);
    frames[idx] |= (0x1 << off);

}

// Static function to clear a bit in the frames bitset

static void clear_frame(u32int frame_addr)
{

    u32int frame = frame_addr/0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int off = OFFSET_FROM_BIT(frame);

    frames[idx] &= ~(0x1 << off);

}

// Static function to test if a bit is set.

static u32int test_frame(u32int frame_addr)
{

    u32int frame = frame_addr/0x1000;
    u32int idx = INDEX_FROM_BIT(frame);
    u32int off = OFFSET_FROM_BIT(frame);

    return (frames[idx] & (0x1 << off));

}

// Static function to find the first free frame.

static u32int first_frame()
{

    u32int i, j;

    for (i = 0; i < INDEX_FROM_BIT(nframes); i++)
    {

        if (frames[i] != 0xFFFFFFFF) // nothing free, exit early.
        {

            // at least one bit is free here.

            for (j = 0; j < 32; j++)
            {

                u32int toTest = 0x1 << j;

                if ( !(frames[i]&toTest) )
                {

                    return i*4*8+j;

                }
            }
        }
    }
}

// Function to allocate a frame.

void alloc_frame(page_t *page, int is_kernel, int is_writeable)
{

    if (page->frame != 0)
    {

        return;

    }
    else
    {

        u32int idx = first_frame();

        if (idx == (u32int)-1)
        {

            // PANIC! no free frames!!

        }

        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writeable==1)?1:0;
        page->user = (is_kernel==1)?0:1;
        page->frame = idx;

    }
}

// Function to deallocate a frame.

void free_frame(page_t *page)
{

    u32int frame;

    if (!(frame=page->frame))
    {

        return;
        
    }
    else
    {

        clear_frame(frame);
        page->frame = 0x0;

    }

}

void initialise_paging()
{

    monitor_write("\nInitializing paging...");

    // The size of physical memory. For the moment we 
    // assume it is 16MB big.

    u32int mem_end_page = 0x1000000;
    
    nframes = mem_end_page / 0x1000;
    frames = (u32int*)kmalloc(INDEX_FROM_BIT(nframes));

    memset(frames, 0, INDEX_FROM_BIT(nframes));
    
    // Let's make a page directory.

    u32int phys;
    kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));

    memset(kernel_directory, 0, sizeof(page_directory_t));

    kernel_directory->physicalAddr = (u32int)kernel_directory->tablesPhysical;

    // Map some pages in the kernel heap area.
    // Here we call get_page but not alloc_frame. This causes page_table_t's 
    // to be created where necessary. We can't allocate frames yet because they
    // they need to be identity mapped first below, and yet we can't increase
    // placement_address between identity mapping and enabling the heap!

    int i = 0;

    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        get_page(i, 1, kernel_directory);

    // We need to identity map (phys addr = virt addr) from
    // 0x0 to the end of used memory, so we can access this
    // transparently, as if paging wasn't enabled.
    // NOTE that we use a while loop here deliberately.
    // inside the loop body we actually change placement_address
    // by calling kmalloc(). A while loop causes this to be
    // computed on-the-fly rather than once at the start.
    // Allocate a lil' bit extra so the kernel heap can be
    // initialised properly.

    i = 0;
    while (i < 0x400000 ) //placement_address+0x1000)
    {

        // Kernel code is readable but not writeable from userspace.

        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
        i += 0x1000;

    }

    // Now allocate those pages we mapped earlier.

    for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000)
        alloc_frame( get_page(i, 1, kernel_directory), 0, 0);

    // Before we enable paging, we must register our page fault handler.

    register_interrupt_handler(14, page_fault);

    // Now, enable paging!

    switch_page_directory(kernel_directory);

    // Initialise the kernel heap.

    kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);

    current_directory = clone_directory(kernel_directory);

    switch_page_directory(current_directory);

    monitor_write(" [done]");

}

void switch_page_directory(page_directory_t *dir)
{

    current_directory = dir;
    asm volatile("mov %0, %%cr3":: "r"(dir->physicalAddr));

    u32int cr0;

    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));

}

page_t *get_page(u32int address, int make, page_directory_t *dir)
{
   
    // Turn the address into an index.
    
    address /= 0x1000;
   
    // Find the page table containing this address.
   
    u32int table_idx = address / 1024;

    if (dir->tables[table_idx]) // If this table is already assigned
    {

        return &dir->tables[table_idx]->pages[address%1024];

    }
    else if(make)
    {

        u32int tmp;
        dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
        memset(dir->tables[table_idx], 0, 0x1000);
        dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.

        return &dir->tables[table_idx]->pages[address%1024];

    }
    else
    {

        return 0;

    }
}


void page_fault(registers_t *regs)
{

    // A page fault has occurred.
    // The faulting address is stored in the CR2 register.

    u32int faulting_address;

    asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
    
    // The error code gives us details of what happened.

    int present   = !(regs->err_code & 0x1); // Page not present
    int rw = regs->err_code & 0x2;           // Write operation?
    int us = regs->err_code & 0x4;           // Processor was in user-mode?
    int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
    int id = regs->err_code & 0x10;          // Caused by an instruction fetch?

    // Output an error message.

    monitor_write("\nPage fault! (");

    if (present) {monitor_write("present");}
    if (rw) {monitor_write("read-only");}
    if (us) {monitor_write("user-mode");}
    if (reserved) {monitor_write("reserved");}

    monitor_write(") at 0x");
    monitor_write_hex(faulting_address);
    monitor_write(" - EIP: ");
    monitor_write_hex(regs->eip);
    monitor_write("\n");

    PANIC("Page fault");

}

static page_table_t *clone_table(page_table_t *src, u32int *physAddr)
{

    // Make a new page table, which is page aligned.

    page_table_t *table = (page_table_t*)kmalloc_ap(sizeof(page_table_t), physAddr);

    // Ensure that the new table is blank.

    memset(table, 0, sizeof(page_directory_t));

    // For every entry in the table...

    int i;

    for (i = 0; i < 1024; i++)
    {

        // If the source entry has a frame associated with it...

        if (!src->pages[i].frame)
            continue;

        // Get a new frame.

        alloc_frame(&table->pages[i], 0, 0);

        // Clone the flags from source to destination.

        if (src->pages[i].present) table->pages[i].present = 1;
        if (src->pages[i].rw)      table->pages[i].rw = 1;
        if (src->pages[i].user)    table->pages[i].user = 1;
        if (src->pages[i].accessed)table->pages[i].accessed = 1;
        if (src->pages[i].dirty)   table->pages[i].dirty = 1;

        // Physically copy the data across. This function is in process.s.

        copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);

    }

    return table;

}

page_directory_t *clone_directory(page_directory_t *src)
{

    u32int phys;

    // Make a new page directory and obtain its physical address.

    page_directory_t *dir = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);

    // Ensure that it is blank.

    memset(dir, 0, sizeof(page_directory_t));

    // Get the offset of tablesPhysical from the start of the page_directory_t structure.

    u32int offset = (u32int)dir->tablesPhysical - (u32int)dir;

    // Then the physical address of dir->tablesPhysical is:

    dir->physicalAddr = phys + offset;

    // Go through each page table. If the page table is in the kernel directory, do not make a new copy.
   
    int i;
    for (i = 0; i < 1024; i++)
    {

        if (!src->tables[i])
            continue;

        if (kernel_directory->tables[i] == src->tables[i])
        {

            // It's in the kernel, so just use the same pointer.

            dir->tables[i] = src->tables[i];
            dir->tablesPhysical[i] = src->tablesPhysical[i];

        }
        else
        {

            // Copy the table.

            u32int phys;
            dir->tables[i] = clone_table(src->tables[i], &phys);
            dir->tablesPhysical[i] = phys | 0x07;

        }

    }

    return dir;
    
}
