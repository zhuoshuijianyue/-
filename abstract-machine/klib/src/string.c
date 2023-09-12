#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
    int count = 0;
    while (*s != '\0') { 
        s++;
        count++;
    }
    return count;
}

char *strcpy(char *dst, const char *src) {
  char *temp=dst;
  while(*src!='\0')
  {
    *dst=*src;
    dst++;
    src++;
  }
  *dst='\0';
  return temp;
}

char *strncpy(char *dst, const char *src, size_t n) {
  for(int i=0;i<n;i++)
  {
    if(i>=strlen(src)) *(dst+i)='\0';
    else *(dst+i)=*(src+i);
  }
  return dst;
}

char *strcat(char *dst, const char *src) {
  int a=strlen(dst);
  int b=strlen(src);
  for(int i=a;i<=a+b+1;i++){
    *(dst+i)=*(src+i-a);
  }
 return dst;
}

int strcmp(const char *s1, const char *s2) {
  while(*s1!='\0'&&*s2!='\0'){
    if(*s1!=*s2){
      return *s1-*s2;
    }
    s1++;
    s2++;
  }
  return *s1-*s2;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  char *temp=(char*) s;
  for(;0<n;n--){
    *temp++=c;
  }
  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  char *a=(char*)s1;
  char *b=(char*)s2;
  for(int i=0;i<n;i++)
  {
    if(*(a+i)!=*(b+i)) return *(a+i)-*(b+i);
  }
  return 0;
}




#endif
