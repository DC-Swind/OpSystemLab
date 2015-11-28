#include "common.h"
#include "protect.h"
#include "printf.h"
#include "drivers.h"
#include "thread.h"
void
echo() {
	lock();
	static int tty = 1;
	printf("tty:%d,pid:%d\n",tty,current->PID);
	char name[] = "tty*", buf[256];
	Device *dev;
	name[3] = '0' + (tty ++);
	unlock();
	
	while (1) {
		dev = hal_get(name);
		//printf("name:%s,dev:%d\n",name,dev);
		if (dev != NULL) {
			//printf("get dev in test\n");
			dev_write(dev, 0, name, 4);
			dev_write(dev, 0, "# ", 2);
			int i, nread = dev_read(dev, 0, buf, 255);
			buf[nread] = 0;
			for (i = 0; i < nread; i ++) {
				if (buf[i] >= 'a' && buf[i] <= 'z') {
					buf[i] += 'A' - 'a';
				}
			}
			dev_write(dev, 0, "Got: ", 5);
			dev_write(dev, 0, buf, nread);
			dev_write(dev, 0, "\n", 1);
		} else {
			//printf("%s\n", name);
		}
		//printf("%s\n", name);
		wait_for_interrupt();
	}
}

void
test() {
	lock();
	int i;
	for (i = 0; i < NR_TTY; i ++) {
		//wakeup(create_kthread(echo));
		create_kthread(echo);
	}
	unlock();
}
