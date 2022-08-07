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

#include <Lunix/kernel/fs.h>

fs_node_t *fs_root = 0; // The root of the filesystem.

u32int read_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer)
{
    // Has the node got a read callback?

    if (node->read != 0)

        return node->read(node, offset, size, buffer);

    else

        return 0;

}

u32int write_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer)
{

    // Has the node got a write callback?

    if (node->write != 0)

        return node->write(node, offset, size, buffer);

    else

        return 0;

}

void open_fs(fs_node_t *node, u8int read, u8int write)
{

    // Has the node got an open callback?

    if (node->open != 0)

        return node->open(node);

}

void close_fs(fs_node_t *node)
{

    // Has the node got a close callback?

    if (node->close != 0)

        return node->close(node);
        
}

struct dirent *readdir_fs(fs_node_t *node, u32int index)
{

    // Is the node a directory, and does it have a callback?

    if ( (node->flags&0x7) == FS_DIRECTORY &&
         node->readdir != 0 )

        return node->readdir(node, index);

    else

        return 0;

}

fs_node_t *finddir_fs(fs_node_t *node, char *name)
{

    // Is the node a directory, and does it have a callback?

    if ( (node->flags&0x7) == FS_DIRECTORY &&
         node->finddir != 0 )

        return node->finddir(node, name);

    else

        return 0;
        
}
