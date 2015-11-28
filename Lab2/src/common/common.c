#include "common.h"

void memcpy(void *dest, const void *src, size_t count){
	char *tmp = dest;
	const char *s = src;
	while (count--) *tmp++ = *s++ ;
}

void memset(void *src, uint8_t data, size_t count){
 assert(src!=NULL);
 char *tmpsrc=(char*)src;
 while(count--) *tmpsrc++ =data;
}

int strcmp(const char* str1, const char* str2)
{
  int ret = 0;
  while(!(ret=*(unsigned char*)str1-*(unsigned char*)str2)&&*str1)
       {
          str1++; str2++;
       }

  if(ret<0) {return -1;}
  else if(ret>0)  {return 1;}
  
  return 0;

}
