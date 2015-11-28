#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include "drivers/hal.h"
#include "drivers/term.h"
#include "drivers/time.h"
#include "drivers/tty.h"
#define OFFSET 0xC0000000
#define pa_to_va(addr) (void *)((uint32_t)addr - OFFSET)

//----------------------test
#define MSG_HWINTR -111
//--------------------------

void add_irq_handle(int irq, void (*func)(void) );
void ttyd(void);
#endif
