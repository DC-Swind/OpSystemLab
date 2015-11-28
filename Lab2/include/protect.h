#ifndef _PROTECT_H
#define _PROTECT_H
#include "device.h"
#include "x86.h"
void int80();
void lock();
void unlock();
void sleep();
void wakeup(Thread *who);
#endif
