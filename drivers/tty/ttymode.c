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
#include <Lunix/type.h>

struct Cursor CursorPos;

void cls()
{

	setcur(0, 0);
	memsetw((unsigned short*) 0xB8000, ((unsigned char) 0x20 | 0x0700), 2000);

}

void setcur(unsigned char x, unsigned char y)
{

	CursorPos.x = x;
	CursorPos.y = y;

	unsigned int index = (80 * y) + x;

	outportb(0x3D4, 14);
	outportb(0x3D5, index >> 8);
	outportb(0x3D4, 15);
	outportb(0x3D5, index);

}

void putch(const char c)
{

	if(c == 0x00)
	{

		return;

	}
	
	if(c == (unsigned char) '\n')
	{

		setcur(0, CursorPos.y + 1);

		return;

	}

	if(c == (unsigned char) '\b')
	{

		if(CursorPos.x == 0)
		{

			if(CursorPos.y >= 8)
			{

				setcur(79, (CursorPos.y - 1));

			}

			else
			{

				return;

			}
		}

		else
		{

			setcur((CursorPos.x - 1), CursorPos.y);

		}

		unsigned short location = 80 * (unsigned short) CursorPos.y;
		location += (unsigned short) CursorPos.x;

		memsetw(((unsigned short*) 0xB8000 + location), 0, 1);

		return;

	}
	
	unsigned short location = 80 * (unsigned short) CursorPos.y;
	location += (unsigned short) CursorPos.x;

	unsigned short character = c | CursorPos.colour;
	
	memsetw(((unsigned short*) 0xB8000 + location), character, 1);

	setcur(CursorPos.x + 1, CursorPos.y);

	if(CursorPos.x >= 80)
	{

		setcur(0, CursorPos.y + 1);

	}
	
}

void puts(const char *s)
{

	size_t len = strlen(s);
	
	for(int i = 0; i < len; i++)
	{

		putch(s[i]);

	}
	
}
