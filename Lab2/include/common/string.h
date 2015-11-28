#ifndef _STRING_H_
#define _STRING_H_
#include "common/types.h"
/*
void *memcpy(void *dest, const void *src, size_t count){
 char *tmp = dest;
 const char *s = src;
 while (count--) *tmp = *s ;
 return dest;
}
void *memset(void *src, int c, size_t count){
 assert(src!=NULL);
 char *tmpsrc=(char*)src;
 while(count--) *tmpsrc++ =(char)c;
 return src;
}
*/
void memcpy(void *dest, const void *src, size_t count);
void memset(void *src, uint8_t data, size_t count);
int strcmp(const char* str1, const char* str2);
#endif
