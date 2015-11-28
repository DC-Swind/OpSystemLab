#include "common.h"
#include "x86.h"
#include "device.h"
#include "printf.h"
#include "thread.h"
#include "protect.h"
#include "drivers.h"
#include "message.h"

void test();
void init_hal();
void init_tty(); 

//-------------------------------------mytest for sem2
/*
int pidA,pidB,pidC,pidD,pidE;
void A () { 
	Message m1, m2;
	m1.src = current->PID;
	int x = 0;
	while(1) {
		if(x % 10000000 == 0) {
			printf("a"); 
			send(pidE, &m1);
			receive(pidE, &m2);
		}
		x ++;
	}
}
void B () { 
	Message m1, m2;
	m1.src = current->PID;
	int x = 0;
	receive(pidE, &m2);
	while(1) {
		if(x % 10000000 == 0) {
			printf("b"); 
			send(pidE, &m1);
			receive(pidE, &m2);
		}
		x ++;
	}
}
void C () { 
	Message m1, m2;
	m1.src = current->PID;
	int x = 0;
	receive(pidE, &m2);
	while(1) {
		if(x % 10000000 == 0) {
			printf("c"); 
			send(pidE, &m1);
			receive(pidE, &m2);
		}
		x ++;
	}
}
void D () { 
	Message m1, m2;
	m1.src = current->PID;
	receive(pidE, &m2);
	int x = 0;
	while(1) {
		if(x % 10000000 == 0) {
			printf("d"); 
			send(pidE, &m1);
			receive(pidE, &m2);
		}
		x ++;
	}
}
 
void E () {
	Message m1, m2;
	m2.src = current->PID;
	char c='1';
	while(1) {
		receive(ANY, &m1);
		if(m1.src == pidA) {c = '|'; m2.dest = pidB; }
		else if(m1.src == pidB) {c = '/'; m2.dest = pidC;}
		else if(m1.src == pidC) {c = '-'; m2.dest = pidD;}
		else if(m1.src == pidD) {c = '\\';m2.dest = pidA;}
		else {printf("m1.src:%d",m1.src); assert(0);} 
 
		printf("\033[s\033[1000;1000H%c\033[u", c);
		send(m2.dest, &m2);
	}
 
}
void mytest(){
	printf("enter mytest!\n");
	Thread *PCB_of_thread_A = create_kthread(A);
	Thread *PCB_of_thread_B = create_kthread(B);
	Thread *PCB_of_thread_C = create_kthread(C);
	Thread *PCB_of_thread_D = create_kthread(D);
	Thread *PCB_of_thread_E = create_kthread(E);
	pidA = PCB_of_thread_A->PID;
pidB = PCB_of_thread_B->PID;
pidC = PCB_of_thread_C->PID;
pidD = PCB_of_thread_D->PID;
pidE = PCB_of_thread_E->PID;
}
*/
//-------------------------------------mytest for sem
/*
Thread *PCB_of_thread_A;
Thread *PCB_of_thread_B;
Thread *PCB_of_thread_C;
Thread *PCB_of_thread_D;

void A () { 
    int x = 0;
    while(1) {
	lock();
        if(x == 0) {
            putchar('a');
	    Message m2;
	    receive(4,&m2);
            Message m1;
	    send(2,&m1);
	    
        }
	unlock();
    }
}
void B () { 
    int x = 0;
    while(1) {
	lock();
        if(x == 0) {
            putchar('b');
	    Message m2;
	    receive(1,&m2);
            Message m1;
	    send(3,&m1);

        }
	unlock();
    }
}
void C () { 
    int x = 0;
    while(1) {
	lock();
        if(x  == 0) {
            putchar('c');
	    Message m2;
	    receive(2,&m2);
            Message m1;
	    send(4,&m1);

        }
	unlock();
    }
}
void D () { 
    int x = 0;
    while(1) {
	lock();
        if(x == 0) {
            putchar('d');
            Message m1;
	    send(1,&m1);
	    Message m2;
	    receive(3,&m2);
        }
	unlock();
    }
}
void mytest(){
	printf("enter mytest!\n");
	PCB_of_thread_A = create_kthread(A);
	PCB_of_thread_B = create_kthread(B);
	PCB_of_thread_C = create_kthread(C);
	PCB_of_thread_D = create_kthread(D);
	printf("A:%d",PCB_of_thread_A->PID);
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
	//mytest();
	test();
}
void
entry(void) {
	init_idt();
	init_intr();
	init_serial();
	//init_threads to init PID = -1, means unuse;
	init_threads();
	current = &Threads_arr[0];
	//------lab2------begin------
		
	Thread *ttyd_thread = create_kthread(ttyd);
	TTY = ttyd_thread->PID;
	printf("TTY-PID:%d\n",TTY);
	
	init_mailbox();
	init_hal();
	init_timer();
	init_tty();

	hal_list();
	
	//------lab2------end------

	enable_interrupt();
	printf("init done\n");
	grading();
	
	while(1){
		//asm volatile("int $0x80");
		//printf("this is idle!\n");
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


