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