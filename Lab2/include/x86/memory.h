#ifndef __X86_MEMORY_H__
#define __X86_MEMORY_H__

#define DPL_KERNEL              0
#define DPL_USER                3

#define NR_SEGMENTS             3
#define SEG_KERNEL_CODE         1 
#define SEG_KERNEL_DATA         2
#include "list.h"
#include "sem.h"
struct GateDescriptor {
	uint32_t offset_15_0      : 16;
	uint32_t segment          : 16;
	uint32_t pad0             : 8;
	uint32_t type             : 4;
	uint32_t system           : 1;
	uint32_t privilege_level  : 2;
	uint32_t present          : 1;
	uint32_t offset_31_16     : 16;
};

struct TrapFrame {
	uint32_t edi, esi, ebp, xxx, ebx, edx, ecx, eax;
	int32_t irq;
	uint32_t eip;
	uint32_t cs;
	uint32_t efl;
	//uint32_t entrytest;
};
typedef struct THREAD {
	struct TrapFrame *tf;
	int PID;
	int lock;
	int sleep;
	int entry;
	//semaphore
	ListHead semq;
	Semaphore sem;
	//mail queue
	ListHead mailq;
	struct THREAD *prev;
	struct THREAD *next;
	uint8_t kstack[4096];
} Thread;

#endif
