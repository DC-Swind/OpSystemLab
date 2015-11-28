#include "x86.h"
#include "device.h"
#include "printf.h"
#include "thread.h"

void A();
void B();
void
irq_handle(struct TrapFrame *tf) {
//	printf("tf:%d,&tf:%d,irq:%d\n",(int)tf,(int)&tf,(int)&tf->irq);
//	printf("\ncs:%d,eip:%d,efl:%d,irq:%d\n",tf->cs,tf->eip,tf->efl,tf->irq);
//	printf("xxx:%d,ebp:%d,int-tf:%d\n",tf->xxx,tf->ebp,(int)tf);

//printf("%d-%d",Thread_N,Thread_Sleep);
if (tf->irq == 1000 || tf->irq == 0x80){
	//----------------------test-------
	//if (tf->irq == 0x80) printf("this is int80\n");
	//---------------------endtest----
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

	//if (current->lock > 0 ) current->tf->efl -= 512;
}else if(tf->irq == 1001) {
	uint32_t code = in_byte(0x60);
	uint32_t val = in_byte(0x61);
	out_byte(0x61, val | 0x80);
	out_byte(0x61, val);
	putchar('0' + code / 100);
	putchar('0' + code / 10 % 10);
	putchar('0' + code % 10);
	putchar('\n');

	//save tf
	current->tf = tf;
} else {
	//save tf
	current->tf = tf;
	printf("\nirq:%d\n",tf->irq);
	assert(0);
}

//	printf("current->lock:%d\n",current->lock);
//	printf("current:%d\n",current->tf->efl);

/*
	if (head == 0){
		Threads_arr[0].tf->eip = tf->eip;
		Threads_arr[0].tf->cs = tf->cs;
		Threads_arr[0].tf->efl = tf->efl;
		tf->eip = Threads_arr[1].tf->eip;
		tf->cs = Threads_arr[1].tf->cs;
		tf->efl = Threads_arr[1].tf->efl;
	}else{
		Threads_arr[1].tf->eip = tf->eip;
		Threads_arr[1].tf->cs = tf->cs;
		Threads_arr[1].tf->efl = tf->efl;
		tf->eip = Threads_arr[0].tf->eip;
		tf->cs = Threads_arr[0].tf->cs;
		tf->efl = Threads_arr[0].tf->efl;

	}

	head = (head + 1)%ThreadNum;
*/	

}

