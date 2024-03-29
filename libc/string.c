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

#include <string.h>
#include <stdint.h>

void int_to_ascii(int n, char str[]) {

    int i, sign;

    if ((sign = n) < 0) n = -n;

    i = 0;

    do {

        str[i++] = n % 10 + '0';

    } while ((n /= 10) > 0);

    if (sign < 0) str[i++] = '-';

    str[i] = '\0';

    reverse(str);
}

void hex_to_ascii(int n, char str[]) {

    append(str, '0');
    append(str, 'x');

    char zeros = 0;

    int32_t tmp;
    int i;

    for (i = 28; i > 0; i -= 4) {

        tmp = (n >> i) & 0xF;

        if (tmp == 0 && zeros == 0) continue;

        zeros = 1;

        if (tmp > 0xA) append(str, tmp - 0xA + 'a');

        else append(str, tmp + '0');

    }

    tmp = n & 0xF;

    if (tmp >= 0xA) append(str, tmp - 0xA + 'a');

    else append(str, tmp + '0');

}

/* K&R */

void reverse(char s[]) {

    int c, i, j;

    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {

        c = s[i];
        s[i] = s[j];
        s[j] = c;

    }

}


char *ctos(char sstring[2], const char c)
{

    sstring[0] = c;
    sstring[1] = '\0';

    return sstring;

}

void append(char s[], char n) {

    int len = strlen(s);

    s[len] = n;
    s[len+1] = '\0';

}

void backspace(char s[]) {

    int len = strlen(s);

    s[len-1] = '\0';

}

// Compare two strings. Should return -1 if 
// str1 < str2, 0 if they are equal or 1 otherwise.

int strcmp(char *str1, char *str2)
{

      int i = 0;
      int failed = 0;

      while(str1[i] != '\0' && str2[i] != '\0')
      {
        
          if(str1[i] != str2[i])
          {

              failed = 1;

              break;

          }

          i++;

      }

      // why did the loop exit?

      if( (str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0') )
          failed = 1;
  
      return failed;

}

// Copy the NULL-terminated string src into dest, and
// return dest.

char *strcpy(char *dest, const char *src)
{

    do
    {

      *dest++ = *src++;

    }

    while (*src != 0);

}

// Concatenate the NULL-terminated string src onto
// the end of dest, and return dest.

char *strcat(char *dest, const char *src)
{

    while (*dest != 0)
    {

        *dest = *dest++;

    }

    do
    {

        *dest++ = *src++;

    }

    while (*src != 0);

    return dest;

}

int strlen(char *src)
{

    int i = 0;

    while (*src++)
        i++;

    return i;

}