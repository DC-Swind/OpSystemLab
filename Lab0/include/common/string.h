#ifndef __STRING_H__
#define __STRING_H__

#include "common.h"

char *itoa(int);
void *memcpy(void *, const void *, size_t);
void *memset(void *, int, size_t);
//need add follows
int memcmp(const void *buf1,const void *buf2,unsigned int count);
#endif
