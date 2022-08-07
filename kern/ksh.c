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
#include <Lunix/kernel/kernel.h>
#include "Lunix/kernel/keyboard.h"
#include <string.h>
#include <mem.h>
#include <stdint.h>

#define BUFFER 64

void LXmonitor(void) {

  char keybbuffer[BUFFER];
	int command = 0;
	uint8_t letter;

    strcpy(&keybbuffer[strlen(keybbuffer)], "");

    monitor_write("\n\nWelcome to the Lunix ksh!\n\n");

    monitor_write("Lunix > ");
    
    while (1)
	  {

		while (letter = scanf())
		{
        
			if (letter == ENTER)
			{

            if (strcmp(keybbuffer, "PANIC") == 0) {

            PANIC("the user forced a kernel panic. Reboot!");

            } else if (strcmp(keybbuffer, "KMALLOC") == 0) {

            uint32_t phys_addr;
            uint32_t page = kmalloc(1000, 1, &phys_addr);
            char page_str[16] = "";
        
            hex_to_ascii(page, page_str);
        
            char phys_str[16] = "";
       
            hex_to_ascii(phys_addr, phys_str);
       
            monitor_write("\n\nPage: ");
            monitor_write(page_str);
            monitor_write(", physical address: ");
            monitor_write(phys_str);
            monitor_write("\n");

            }

            else if (strcmp(keybbuffer, "HELP") == 0) {

            monitor_write("\n\nThe kernel monitor allows you to:\n");
            monitor_write("\n1: Test kernel functions");
            monitor_write("\n2: Test the hardware response");
            monitor_write("\nTry type 'CMD' to see the available commands.\n");
    
            }

            else if (strcmp(keybbuffer, "CMD") == 0) {

            monitor_write("\n\nAvailable Lunix monitor commands:\n");
            monitor_write("\nPANIC      - Force a kernel panic");
            monitor_write("\nKMALLOC    - Test the Lunix kmalloc()");
            monitor_write("\nHELP       - See the help");
            monitor_write("\nCMD        - View this help");
            monitor_write("\nREBOOT     - Reboot the device");
            monitor_write("\nSHUTDOWN   - Shutdown the device");
            monitor_write("\nUSER       - See who are logged in");
            monitor_write("\nCLEAR      - Clear the screen\n");
    
            }

            else if (strcmp(keybbuffer, "REBOOT") == 0) {

            LXreboot();
    
            }

            else if (strcmp(keybbuffer, "SHUTDOWN") == 0) {

            LXshutdown();
    
            }

            else if (strcmp(keybbuffer, "USER") == 0) {

            char *userID=LXget_user(1);

            monitor_write("\n\nUser ID: ");
            monitor_write(userID);
            monitor_write("\n");

            }

            else if (strcmp(keybbuffer, "CLEAR") == 0) {

            clear_screen();
        
            }

            monitor_write("\nLunix > ");

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

				monitor_write(s);

				keybbuffer[strlen(keybbuffer) - 1] = '\0';

			}

			else
			{

				char c = resolvChar(letter);
				char *s;

				s = ctos(s, c);

				monitor_write(s);

				strcpy(&keybbuffer[strlen(keybbuffer)], s);

	    command = 0;

	    }

        }

    }

}

