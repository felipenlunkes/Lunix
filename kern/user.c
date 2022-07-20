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

#include <Lunix/console.h>
#include <Lunix/kernel/user.h>
#include "../libc/string.h"
#include "../libc/mem.h"
#include <stdint.h>

struct {

char name[15];
int ID;
int groupID;
int root;
int tty;

} users[5]; 

int LXid_user(int tty){



}

char *LXget_user(int tty){

    char *user[64];

    memory_copy(user, users[tty].name, 64);

    return user;

}

int LXget_userID(int tty){

    return users[tty].ID;
    
}

void LXset_user(int tty, char *name, int ID, int groupID){

    kprint("\nLunix: user '");
    kprint(name);
    kprint("' logged in.\n");

    users[tty].tty=tty;
    memory_copy(users[tty].name, *name, 15);
    users[tty].ID=ID;
    users[tty].groupID=groupID;

    if (ID=777) {

        users[tty].root=1;

    } else {

        users[tty].root=0;

    }

}