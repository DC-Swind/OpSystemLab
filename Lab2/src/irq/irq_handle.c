#include "x86.h"
#include "device.h"
#include "printf.h"
#include "thread.h"
int is_irq_or_not = 0;
//--------------------------------------------------------
#define NR_IRQ_HANDLE 32
#define NR_HARD_INTR 16

struct IRQ_t {
	void (*routine)(void);
	struct IRQ_t *next;
};

static struct IRQ_t handle[NR_IRQ_HANDLE];
static struct IRQ_t *handles[NR_HARD_INTR];
static int handle_count = 0;

void add_irq_handle(int irq, void (*func)(void) ) {
	struct IRQ_t *ptr;
	assert(irq < NR_HARD_INTR);
	if (handle_count > NR_IRQ_HANDLE) {
		printf("Too many irq handle!");
	}
	ptr = &handle[handle_count ++];
	ptr->routine = func;
	ptr->next = handles[irq]; 
	handles[irq] = ptr;
}

//-------------------------------------------------------------------
void A();
void B();
void irq_handle(struct TrapFrame *tf) {
	is_irq_or_not = 1;
	//printf("%d",current->PID);
if (tf->irq == 1000 || tf->irq == 0x80){
	//if (current->PID == 3 || current->PID ==5){
	//	printf("%d\n",*current->tf);
	//}
	if (current == NULL || current == &Threads_arr[0] ||(Thread_N - Thread_Sleep <= 0)){
		if (current == NULL) Threads_arr[0].tf = tf;
		else current->tf = tf;

		if (Thread_N - Thread_Sleep >0){
			current = threadlist;
			while(current->sleep == 1){
				current = current->next;
			}
		}else current = &Threads_arr[0];
	}else{
		//the stack's content is change
		current->tf = tf;   
		do{
			current = current->next;
		}while(current->sleep == 1);
		
	}	
	//----------------------Lab2-----------------------

	if(tf->irq == 1000){
		struct IRQ_t *f = handles[0];
		while(f != NULL){
			f->routine();
			f = f->next;
		}
	}

	//---------------------endLab2---------------------
}else if(tf->irq == 1001) {
	/*
	uint32_t code = in_byte(0x60);
	uint32_t val = in_byte(0x61);
	out_byte(0x61, val | 0x80);
	out_byte(0x61, val);
	putchar('0' + code / 100);
	putchar('0' + code / 10 % 10);
	putchar('0' + code % 10);
	putchar('\n');
	*/
	//---------------------Lab2-----------------------------
	struct IRQ_t *f = handles[1];
	while(f != NULL){
		f->routine();
		f = f->next;
	}
	//---------------------endLab2--------------------------
	//save tf
	current->tf = tf;
} else {
	//save tf
	current->tf = tf;
	printf("\nirq:%d\n",tf->irq);
	assert(0);
}
	is_irq_or_not = 0;
}
