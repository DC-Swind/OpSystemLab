#ifndef __SEM_H__
#define __SEM_H__
#include "list.h"
struct Semaphore {
	int count;
	ListHead queue;
};
typedef struct Semaphore Semaphore;
void V(Semaphore *sem);
void P(Semaphore *sem);
void
init_sem(Semaphore *sem, int value);
#endif
