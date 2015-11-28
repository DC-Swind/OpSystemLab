#include "sem.h"
#include "x86.h"
#include "protect.h"
#include "list.h"
#include "thread.h"
void
init_sem(Semaphore *sem, int value) {
	sem->count = value;
	list_init(&sem->queue);
}

void
P(Semaphore *sem) {
	lock();
	sem->count --;
	if (sem->count < 0) {
		list_add_before(&sem->queue, &current->semq); // You should add ListHead semq in the Thread structure
		sleep(); // sleep!
	}
	unlock();
}

void
V(Semaphore *sem) {
	lock();
	sem->count ++;
	if (sem->count <= 0) {
		assert(!list_empty(&sem->queue));
		Thread *t = list_entry(sem->queue.next, Thread, semq);
		list_del(sem->queue.next);
		wakeup(t);
	}
	unlock();
}
