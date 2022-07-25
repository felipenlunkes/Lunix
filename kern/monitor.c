/*
Copyright (c) 2018, Carlos Fenollosa
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
#include <string.h>
#include <mem.h>
#include "Lunix/kernel/keyboard.h"
#include <Lunix/kernel/kernel.h>
#include <Lunix/kernel/thread.h>
#include <stdint.h>

#define BUFFER 64

void LXinit_user_task(){

int setUser=0;

    kprint("\n\nWelcome to the Lunix kernel monitor prompt\n");

    LXset_user(1, "root", 777, 1);

    kprint("\n");

// Start the Lunix kernel monitor task

    LXmonitor("root");

    yield();

    for (;;);

}

void LXmonitor(char *input) {

    char keybbuffer[BUFFER];
	int command = 0;
	uint8_t letter;

    strcpy(&keybbuffer[strlen(keybbuffer)], "");

    kprint("Lunix > ");
    
    while (1)
	{
		while (letter = scanf())
		{
			if (letter == ENTER)
			{

            if (strcmp(keybbuffer, "PANIC") == 0) {

            panic("the user forced a kernel panic. Reboot\n");

            } else if (strcmp(keybbuffer, "KMALLOC") == 0) {

            uint32_t phys_addr;
            uint32_t page = kmalloc(1000, 1, &phys_addr);
            char page_str[16] = "";
        
            hex_to_ascii(page, page_str);
        
            char phys_str[16] = "";
       
            hex_to_ascii(phys_addr, phys_str);
       
            kprint("\n\nPage: ");
            kprint(page_str);
            kprint(", physical address: ");
            kprint(phys_str);
            kprint("\n");

            }

            else if (strcmp(keybbuffer, "HELP") == 0) {

            kprint("\nThe kernel monitor allows you to:\n");
            kprint("\n1: Test kernel functions");
            kprint("\n2: Test the hardware response");
            kprint("\nTry type 'CMD' to see the available commands.\n\n");
    
            }

            else if (strcmp(keybbuffer, "CMD") == 0) {

            kprint("\nAvailable Lunix monitor commands:\n");
            kprint("\nPANIC      - Force a kernel panic");
            kprint("\nKMALLOC    - Test the Lunix kmalloc()");
            kprint("\nHELP       - See the help");
            kprint("\nCMD        - View this help");
            kprint("\nREBOOT     - Reboot the device");
            kprint("\nSHUTDOWN   - Shutdown the device");
            kprint("\nUSER       - See who are logged in");
            kprint("\nCLEAR      - Clear the screen\n\n");
    
            }

            else if (strcmp(keybbuffer, "REBOOT") == 0) {

            LXreboot();
    
            }

            else if (strcmp(keybbuffer, "SHUTDOWN") == 0) {

            LXshutdown();
    
            }

            else if (strcmp(keybbuffer, "USER") == 0) {

            int userID=LXget_userID(1);

            kprint("\nUser ID: ");
            kprint(userID);

            }

            else if (strcmp(keybbuffer, "CLEAR") == 0) {

            clear_screen();
        
            }

            kprint("\nLunix > ");

			memory_set(keybbuffer, 0, BUFFER);

			break;

			}

			else if ((letter == BACKSPACE) && (strlen(keybbuffer) == 0))
			{

			}

			else if (letter == BACKSPACE)
			{

				char c = resolvChar(letter);
				char *s;

				s = ctos(s, c);

				kprint(s);

				keybbuffer[strlen(keybbuffer) - 1] = '\0';

			}

			else
			{

				char c = resolvChar(letter);
				char *s;

				s = ctos(s, c);

				kprint(s);

				strcpy(&keybbuffer[strlen(keybbuffer)], s);

	    command = 0;

	    }

        }

    }

}

