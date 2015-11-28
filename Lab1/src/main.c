#include "common.h"
#include "x86.h"
#include "device.h"
#include "printf.h"
#include "thread.h"
#include "protect.h"
/*
Thread *PCB_of_thread_A;
Thread *PCB_of_thread_B;
Thread *PCB_of_thread_C;
Thread *PCB_of_thread_D;

void A () { 
    int x = 0;
    while(1) {
	lock();
        if(x % 100000 == 0) {
            putchar('a');
            wakeup(PCB_of_thread_B);
            sleep();
        }
        x = (x+1) % 200000;
	unlock();
    }
}
void B () { 
    int x = 0;
    while(1) {
	lock();
        if(x % 100000 == 0) {
            putchar('b');
            wakeup(PCB_of_thread_C);
            sleep();
        }
             x = (x+1) % 200000;
	unlock();
    }
}
void C () { 
    int x = 0;
    while(1) {
	lock();
        if(x % 100000 == 0) {
            putchar('c');
            wakeup(PCB_of_thread_D);
            sleep();
        }
             x = (x+1) % 200000;
	unlock();
    }
}
void D () { 
    int x = 0;
    while(1) {
	lock();
        if(x % 100000 == 0) {
            putchar('d');
            wakeup(PCB_of_thread_A);
            sleep();
        }
             x = (x+1) % 200000;
	unlock();
    }
}
void grading(){
	printf("enter grading!\n");
	PCB_of_thread_A = create_kthread(A);
	PCB_of_thread_B = create_kthread(B);
	PCB_of_thread_C = create_kthread(C);
	PCB_of_thread_D = create_kthread(D);
	
}
*/
/*
Thread *threadA;
void A(){
	int i=0;
	int j;
for (j=0;j<100;j++){
		lock();
		lock();
		i++;
		putchar('a');
		sleep();
		unlock();
		unlock();
	}
}
void B(){
	int i=0;
	int j;
for (j=0; j< 110; j++){
		i++;
		putchar('b');
		wakeup(threadA);
	
		
		wait_for_interrupt();
	}
}
void C(){
	int i =0;
	while(1){
		i++;
		putchar('c');
		if (i == 20){ 
			printf("\nstop C\n");
			stop_thread();
		}
		wait_for_interrupt();
	}	
}
*/
void grading()
{

}
void
entry(void) {
	init_timer();
	init_idt();
	init_intr();
	init_serial();
	//init_threads to init PID = -1, means unuse;
	init_threads();
	current = &Threads_arr[0];
	enable_interrupt();
	printf("init done\n");
	grading();
	while(1){
		//asm volatile("int $0x80");
		printf("this is idle!\n");
		wait_for_interrupt();
	}
	/*
	threadA = create_kthread(A);
	Thread *threadB = create_kthread(B);
	Thread *threadC = create_kthread(C);
	printf("A:%d,%d,%d\n",threadA->tf->eip,A,threadA->PID);
	printf("B:%d,%d,%d\n",threadB->tf->eip,B,threadB->PID);
	printf("C:%d,%d,%d\n",threadC->tf->eip,C,threadC->PID);
	
	printf("order:%d,%d,%d\n",threadlist->PID,threadlist->next->PID,threadlist->next->next->PID);
	//current = threadlist;
	enable_interrupt();
	int i = 3;
	while (1) {
		i++;
		if (i % 20 == 0) create_kthread(C);
		wait_for_interrupt();
	}
*/

	printf("\nI can not believe that!\n");
	assert(0);
}


