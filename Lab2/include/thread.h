#ifndef _THREAD_H
#define _THREAD_H

#define MAX_THREADS_N 200
extern Thread *current;
extern volatile int Thread_N;
extern volatile int Thread_Sleep;
extern Thread Threads_arr[];
extern Thread* threadlist;
extern Thread* threadtail;

Thread *create_kthread(void(*entry)(void));
void g();
void stop_thread();
void init_threads();
#endif
