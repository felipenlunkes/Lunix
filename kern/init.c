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
#include <Lunix/kernel/timer.h>
#include <Lunix/kernel/paging.h>
#include <Lunix/kernel/multiboot.h>
#include <Lunix/kernel/fs.h>
#include <Lunix/kernel/initrd.h>
#include <Lunix/kernel/task.h>
#include <Lunix/kernel/syscall.h>
#include <Lunix/kernel/version.h>

void ls_initrd(void)
{

    monitor_write("\nOpening the initrd...\n");

// Contents of /

    int i = 0;
    struct dirent *node = 0;

    while ( (node = readdir_fs(fs_root, i)) != 0)
    {

    monitor_write(" > Found file ");
    monitor_write(node->name);

    fs_node_t *fsnode = finddir_fs(fs_root, node->name);

    if ((fsnode->flags&0x7) == FS_DIRECTORY)
    monitor_write(" (directory)\n");

  i++;

}

}

void ls_initrd_files(void)
{

    monitor_write("\nOpening the initrd files...\n");
    
// Contents of /

    int i = 0;
    struct dirent *node = 0;

    while ( (node = readdir_fs(fs_root, i)) != 0)
    {

    monitor_write("Found file ");
    monitor_write(node->name);

    fs_node_t *fsnode = finddir_fs(fs_root, node->name);

    if ((fsnode->flags&0x7) == FS_DIRECTORY)
    monitor_write(" (directory)\n");

    else
    {

    monitor_write("\n\t contents: \"");

    char buf[256];

    u32int sz = read_fs(fsnode, 0, 256, buf);

    int j;

    for (j = 0; j < sz; j++)

      monitor_put(buf[j]);

    monitor_write("\"\n");

  }

  i++;

}

}

void initialise_devices(void){

  init_COM1(); // Serial port init
  init_Parallel(); // Parallel port init
  init_keyboard();

}