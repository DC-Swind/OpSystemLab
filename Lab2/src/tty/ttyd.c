#include "drivers.h"
#include "protect.h"
#include "printf.h"
#include "thread.h"
void
ttyd(void) {
	Message m;

	while (1) {
		receive(ANY, &m);
		if (m.src == MSG_HWINTR) {
			switch (m.type) {
				case MSG_TTY_GETKEY:
					readkey();
					break;
				case MSG_TTY_UPDATE:
					update_banner();
					break;
			}
		} else {
			//wrong
			//Message *msg;
			//printf("ttyd else,m.type:%d!\n",m.type);
			DevMessage *msg;
			switch(m.type) {
				case MSG_DEVRD:
					read_request((DevMessage*)&m);
					break;
				case MSG_DEVWR:
					msg = (DevMessage*)&m;
					//printf("ttyd,dev_id:%d\n",msg->dev_id);
					msg->dev_id = 0;
					if (msg->dev_id >= 0 && msg->dev_id < NR_TTY) {
						int i;
						for (i = 0; i < msg->count; i ++) {
							consl_writec(&ttys[msg->dev_id], ((char*)msg->buf)[i]);
						}
						consl_sync(&ttys[msg->dev_id]);
					}
					m.type = msg->count;
					//-----------------------------lab2-----
					m.src = current->PID;
					//-------------------------------------
					send(m.src, &m);
					break;
			}
		}
	}
}

