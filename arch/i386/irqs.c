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

#include <Lunix/io.h>
#include <Lunix/console.h>
#include <Lunix/ports.h>

struct Cursor CursorPosIRQS;

unsigned char keymap[89] = 
{

	0,
	0x1B,
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0',
	'-',
	'=',
	'\b',
	'\t',
	'q',
	'w',
	'e',
	'r',
	't',
	'y',
	'u',
	'i',
	'o',
	'p',
	'[',
	']',
	'\n',
	0,
	'a',
	's',
	'd',
	'f',
	'g',
	'h',
	'j',
	'k',
	'l',
	';',
	'\'',
	'`',
	0,
	'\\',
	'z',
	'x',
	'c',
	'v',
	'b',
	'n',
	'm',
	',',
	'.',
	'/',
	0,
	'*',
	0,
	' ',
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	'-',
	0,
	0,
	0,
	'+',
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,

};

unsigned char keymapshift[89] = 
{

	0,
	0x1B,
	'!',
	'\"',
	'$',
	'$',
	'%',
	'^',
	'&',
	'*',
	'(',
	')',
	'_',
	'+',
	'\b',
	'\t',
	'Q',
	'W',
	'E',
	'R',
	'T',
	'Y',
	'U',
	'I',
	'O',
	'P',
	'{',
	'}',
	'\n',
	0,
	'A',
	'S',
	'D',
	'F',
	'G',
	'H',
	'J',
	'K',
	'L',
	':',
	'@',
	'`',
	0,
	'|',
	'Z',
	'X',
	'C',
	'V',
	'B',
	'N',
	'M',
	'<',
	'>',
	'?',
	0,
	'*',
	0,
	' ',
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	'-',
	0,
	0,
	0,
	'+',
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,

};

unsigned char keystatus = 0x00; /* Bit 7 - Left Shift
				   Bit 6 - Right Shift
				   Bit 5 - Left Ctrl
				   Bit 4 - Right Ctrl
				   Bit 3 - Scroll lock
				   Bit 2 - Num lock
				   Bit 1 - 0
				   Bit 0 - 0 */

void CIRQ0()
{

	ticks++;

	/* int x = CursorPosIRQS.x;
	int y = CursorPosIRQS.y;
	setcur(46, 24);
	char buf[10];
	puts("0x");
	puts(itoa((ticks), buf, 10));
	puts(" ticks have passed...");
	setcur(x, y); */
		
	outportb(0x20, 0x20); /* EOI */

}

void CIRQ1()
{

/*

	unsigned char scancode = inportb(0x60);

	// TODO: Replace the following with a select..case 
	
	if(scancode & 0x80)
	{

		if( (scancode == 0xAA) || (scancode == 0xB6) )
		{

			if(scancode == 0xAA)
			{

				keystatus -= 128;

			}

			else
			{

				keystatus -= 64;

			}

		}
		
		outportb(0x20, 0x20);

		return;

	}

	switch(scancode)
	{

		case 0x2A:

			keystatus |= 0x80;
			break;

		case 0x36:

			keystatus |= 0x40;
			break;

		case 0x47:

			setcur(0, CursorPosIRQS.y);
			break;

		case 0x4F:

			setcur(79, CursorPosIRQS.y);
			break;

		case 0x4B:

			if(CursorPosIRQS.x > 0)
			{

				setcur((CursorPosIRQS.x - 1), CursorPosIRQS.y);

			}

			else
			{

				setcur(79, (CursorPosIRQS.y - 1));

			}

			break;

		case 0x4D:
			if(CursorPosIRQS.x < 79)
			{

				setcur((CursorPosIRQS.x + 1), CursorPosIRQS.y);

			}

			else
			{

				setcur(0, (CursorPosIRQS.y + 1));
			}

			break;

		case 0x48:

			if(CursorPosIRQS.y >= 8)
			{

				setcur(CursorPosIRQS.x, (CursorPosIRQS.y - 1));

			}

			break;

		case 0x50:

			if(CursorPosIRQS.y <= 23)
			{

				setcur(CursorPosIRQS.x, (CursorPosIRQS.y + 1));

			}

			break;

		default:

			if( (keystatus & 0x80) || (keystatus & 0x40) )
			{

				putch(keymapshift[scancode]);

			}

			else
			{

				putch(keymap[scancode]);

			}

			break;

	}
		
	outportb(0x20, 0x20); //

*/

}

void CIRQ7()
{

	/* This IRQ is raised by the PIC if during the interrupt sequence,
	   IRn is not raised for the correct length of time:
	   	1. IRn raised high - IRRn set in PIC
		2. PIC evaluates - sends INT to CPU if required
		3. CPU drops INTA-bar low to acknowledge
		4. PIC receives INTA-bar low
	   The 'correct length of time' being defined as step 4 here */

	wait();
	outportb(0x20, 0x20); /* EOI */

}

