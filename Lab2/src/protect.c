#include "common.h"
#include "x86.h"
#include "device.h"
#include "thread.h"
#include "protect.h"
#include "printf.h"
#include "message.h"
#include "sem.h"
#include "drivers.h"
#include "list.h"
extern int is_irq_or_not;
int mailboxn;
typedef struct MAILBOX{
	Message m;
	int v;
} MAILBOX;

MAILBOX Mailbox[MAILBOX_SIZE];
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

void init_mailbox(){
	int i;
	for(i=0; i<MAILBOX_SIZE; i++){ 
		Mailbox[i].v = 0;
	}
	mailboxn = 0;
}

void send(pid_t dest, Message *m) {
	lock();
	int i=0;
	if(mailboxn>=MAILBOX_SIZE) printf("MAILBOX Overflow!\n");
	assert(mailboxn<MAILBOX_SIZE);
	if (dest>=0 && Threads_arr[dest].PID==-1) return;   //no this thread
	while(Mailbox[i].v) i++;
	Mailbox[i].v=1;
	m->dest = dest; 
  	//if (is_irq_or_not) m->src = MSG_HWINTR;  //from interrupt	
  	//else m->src = current->PID; 
  	memcpy(&(Mailbox[i]), m, sizeof(Message));
  	mailboxn++;
  	list_add_before(&(Threads_arr[dest].mailq), &(Mailbox[i].m.list));
  	V(&(Threads_arr[dest].sem));
	//wakeup(&Threads_arr[dest]);  	
	unlock();
}

void receive(pid_t src, Message *m) {
	lock();
	ListHead *i;
	Message *mitr;
  	while(1) {
    		if(list_empty(&(current->mailq))){
			P(&(current->sem));
			//sleep();
			continue;
		}
    		list_foreach(i, &(current->mailq)) {
      			mitr=list_entry(i, Message, list);
      			if(src==ANY || mitr->src==src) {
        			memcpy(m, mitr, sizeof(Message));
        			list_del(i);
        			((MAILBOX*)mitr)->v=0;
        			mailboxn--;
        			unlock();
        			return;
      			}
    		}
		P(&(current->sem));
		//sleep();
  	}
}

