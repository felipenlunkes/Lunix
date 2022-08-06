#ifndef STRINGS_H
#define STRINGS_H

void int_to_ascii(int n, char str[]);
char *strcpy(char *dest, const char *src);
void hex_to_ascii(int n, char str[]);
void reverse(char s[]);
int strlen(char s[]);
void backspace(char s[]);
void append(char s[], char n);
int strcmp(char s1[], char s2[]);
char *ctos(char sstring[2], const char c);
char *dtoc(int, char[]);
char *itoa(long, char[], int);

#endif
