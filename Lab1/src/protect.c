#include "common.h"
#include "x86.h"
#include "device.h"
#include "thread.h"
#include "protect.h"
#include "printf.h"
void int80(){
	asm volatile("int $0x80");	
}
void lock(){
	disable_interrupt();
	(current->lock)++;
}

void unlock(){
	if (current->lock > 0) (current->lock)--;
	//use this way to read efl is not right,because the tf is change
	if (current->lock == 0){
 		enable_interrupt();
		//printf("unlock:\n");
	}
}

void sleep(){
	lock();
	if (current->sleep == 0) Thread_Sleep++;
	current->sleep = 1;
	int80();
	unlock();
	//enable_interrupt();
	//wait_for_interrupt();
}
void wakeup(Thread *who){
	lock();
	if (who->sleep == 1) Thread_Sleep--;
	who->sleep = 0;
	unlock();
}
