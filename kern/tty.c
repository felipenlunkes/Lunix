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

// The VGA framebuffer starts at 0xB8000.

u16int *video_memory = (u16int *)0xB8000;

// Stores the cursor position.

u8int cursor_x = 0;
u8int cursor_y = 0;

// Updates the hardware cursor.

static void move_cursor()
{

    // The screen is 80 characters wide...

    u16int cursorLocation = cursor_y * 80 + cursor_x;
    
    outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
    outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D5, cursorLocation);      // Send the low cursor byte.

}

// Scrolls the text on the screen up by one line.

static void scroll()
{

    // Get a space character with the default colour attributes.

    u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 /* space */ | (attributeByte << 8);

    // Row 25 is the end, this means we need to scroll up

    if(cursor_y >= 25)
    {

        // Move the current text chunk that makes up the screen
        // back in the buffer by a line

        int i;

        for (i = 0*80; i < 24*80; i++)
        {

            video_memory[i] = video_memory[i+80];

        }

        // The last line should now be blank. Do this by writing
        // 80 spaces to it.

        for (i = 24*80; i < 25*80; i++)
        {

            video_memory[i] = blank;

        }

        // The cursor should now be on the last line.

        cursor_y = 24;

    }
}

// Writes a single character out to the screen.
void monitor_put(char c)
{

    // The background colour is black (0), the foreground is white (15).

    u8int backColour = 0;
    u8int foreColour = 15;

    // The attribute byte is made up of two nibbles - the lower being the 
    // foreground colour, and the upper the background colour.

    u8int  attributeByte = (backColour << 4) | (foreColour & 0x0F);

    // The attribute byte is the top 8 bits of the word we have to send to the
    // VGA board.

    u16int attribute = attributeByte << 8;
    u16int *location;

    // Handle a backspace, by moving the cursor back one space

    if (c == 0x08 && cursor_x)
    {

        cursor_x--;

    }

    // Handle a tab by increasing the cursor's X, but only to a point
    // where it is divisible by 8.

    else if (c == 0x09)
    {

        cursor_x = (cursor_x+8) & ~(8-1);

    }

    // Handle carriage return

    else if (c == '\r')
    {

        cursor_x = 0;

    }

    // Handle newline by moving cursor back to left and increasing the row

    else if (c == '\n')
    {

        cursor_x = 0;
        cursor_y++;

    }

    // Handle any other printable character.

    else if(c >= ' ')
    {

        location = video_memory + (cursor_y*80 + cursor_x);
        *location = c | attribute;
        cursor_x++;

    }

    // Check if we need to insert a new line because we have reached the end
    // of the screen.

    if (cursor_x >= 80)
    {

        cursor_x = 0;
        cursor_y ++;

    }

    // Scroll the screen if needed.

    scroll();

    // Move the hardware cursor.

    move_cursor();

}

// Clears the screen, by copying lots of spaces to the framebuffer.

void monitor_clear()
{
    // Make an attribute byte for the default colours

    u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    u16int blank = 0x20 /* space */ | (attributeByte << 8);

    int i;

    for (i = 0; i < 80*25; i++)
    {

        video_memory[i] = blank;

    }

    // Move the hardware cursor back to the start.

    cursor_x = 0;
    cursor_y = 0;

    move_cursor();

}

// Outputs a null-terminated ASCII string to the monitor.

void monitor_write(char *c)
{

    int i = 0;

    while (c[i])
    {

        monitor_put(c[i++]);

    }

}

void monitor_write_hex(u32int n)
{

    s32int tmp;

    monitor_write("0x");

    char noZeroes = 1;

    int i;

    for (i = 28; i > 0; i -= 4)
    {

        tmp = (n >> i) & 0xF;

        if (tmp == 0 && noZeroes != 0)
        {

            continue;

        }
    
        if (tmp >= 0xA)
        {

            noZeroes = 0;

            monitor_put (tmp-0xA+'a' );

        }
        else
        {

            noZeroes = 0;

            monitor_put( tmp+'0' );

        }

    }
  
    tmp = n & 0xF;

    if (tmp >= 0xA)
    {

        monitor_put (tmp-0xA+'a');

    }
    else
    {

        monitor_put (tmp+'0');

    }

}

void monitor_write_dec(u32int n)
{

    if (n == 0)
    {

        monitor_put('0');

        return;

    }

    s32int acc = n;
    char c[32];
    int i = 0;

    while (acc > 0)
    {
        
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;

    }

    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;

    while(i >= 0)
    {

        c2[i--] = c[j++];

    }
    
    monitor_write(c2);

}
