#include "common.h"
#include "x86.h"
#include "device.h"
#include "thread.h"
#include "printf.h"
#include "protect.h"

#define MAX_THREADS_N 200

volatile Thread *current = NULL;
volatile int Thread_N = 0;
volatile int Thread_Sleep = 0;
Thread Threads_arr[MAX_THREADS_N];
Thread * threadlist = NULL;
Thread * threadtail = NULL;

void init_threads(){
	int i;
	//save as idle
	Threads_arr[0].PID = 0;
	for (i = 1; i<MAX_THREADS_N; i++) Threads_arr[i].PID = -1;
}
void stop_thread()
{
	lock();
	//idle can not stop
	if (current->PID == 0){ unlock(); return; } 
	current->PID = -1;
	Thread_N--;
	if (current->sleep == 1) Thread_Sleep--;
	if (current->prev == current){
		threadlist = NULL;
		threadtail = NULL;
	}else{
		current->next->prev = current->prev;
		current->prev->next = current->next;
		if (current == threadtail) threadtail = current->prev;
		//current's change is in irq_handle, is it right?
	}
	int80();
	unlock();
	//enable_interrupt();
	//wait_for_interrupt();
}

void g(){
	((void (*)())current->entry)();
	stop_thread();
	printf("g error!\n");
	assert(0);
}

Thread *create_kthread(void(*entry)(void)){
	lock();
	int i;
	Thread *thread = NULL;
	for (i = 0; i < MAX_THREADS_N; i++) if( Threads_arr[i].PID == -1){
		thread = &Threads_arr[i];
		break;
	}
	if (thread == NULL) printf("No thread!!!\n");
	thread->PID = i;
	Thread_N++;
	//thread->tf = (struct TrapFrame *)((int)&thread->kstack[4095]);
	thread->tf = ((struct TrapFrame *)(thread->kstack + 4096))-1;
	thread->tf->edi = 0;
	thread->tf->esi = 0;
	thread->tf->ebp = 0;
	thread->tf->xxx = (int)thread->tf +32;
	thread->tf->ebx = 0;
	thread->tf->edx = 0;
	thread->tf->ecx = 0;
	thread->tf->eax = 0;
	thread->tf->eip = (int)g;
	thread->tf->cs = 8;
	thread->tf->efl = 514;
	thread->entry = (int)entry;
	thread->lock = 0;
	thread->sleep = 0;
	
	if (threadlist == NULL){ 
		threadlist = thread;
		threadlist->prev = thread;
		threadlist->next = thread;
		threadtail = threadlist;
	}else{
		thread->next = threadtail->next;
		thread->prev = threadtail;
		threadtail->next = thread;
		threadtail = thread;
	}
	unlock();
	return thread;	
}


