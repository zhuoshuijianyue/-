#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

void intToString(int num, char str[], int strSize) {
    int i = 0;
    int isNegative = 0;

    // Handle negative numbers
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // Convert digits to characters and store in the string buffer
    do {
        str[i++] = num % 10 + '0';
        num /= 10;
    } while (num > 0 && i < strSize - 1);

    // Add the negative sign if necessary
    if (isNegative && i < strSize - 1) {
        str[i++] = '-';
    }

    // Add the null-terminator
    str[i] = '\0';

    // Reverse the string to get the correct order of digits
    int start = isNegative ? 1 : 0;
    int end = i - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

int printf(const char *fmt, ...) {
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int temp=0;
    strcpy(out,"\0");
    char str[1024];
    for(int i=0;i<=strlen(fmt);i++)
    {
        if(*(fmt+i)=='%') {
        if(*(fmt+i+1)=='s') strcat(out,va_arg(args,char*));
        if(*(fmt+i+1)=='d') {
            intToString(va_arg(args,int),str,sizeof(str) );
            strcat(out,str);}
        i++; 
        }
        else {
            temp=strlen(out);
            out[temp]=*(fmt+i);
            out[temp+1]='\0';
        }
  }
  va_end(args);
  if(out==NULL) return -1;
  else return (strlen(out));
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
