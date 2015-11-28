#ifndef __SEM_H__
#define __SEM_H__
struct Semaphore {
	int count;
	ListHead queue;
};
typedef struct Semaphore Semaphore;
#endif