#define NR_DEV 64
#include "drivers.h"
#include "list.h"
#include "protect.h"
#include "printf.h"
#include "thread.h"
static Device dev_pool[NR_DEV];
static ListHead free, devices;


void init_hal(void) {
	int i = 0;
	list_init(&free);
	for (i = 0; i < NR_DEV; i ++) {
		list_add_before(&free, &dev_pool[i].list);
	}
	list_init(&devices);
}

void hal_register(const char *name, pid_t pid, int dev_id) {
	lock();
	if (list_empty(&free)) {
		printf("no room for more device");
	}
	Device *dev = list_entry(free.next, Device, list);
	list_del(&dev->list);
	dev->name = name;
	dev->pid = pid;
	dev->dev_id = dev_id;
	list_add_before(&devices, &dev->list);
	unlock();
}

void hal_list(void) {
	//lock();
	ListHead *it;
	printf("listing all registed devices:\n");
	list_foreach(it, &devices) {
		Device *dev = list_entry(it, Device, list);
		printf("%s #%d, #%d\n", dev->name, dev->pid, dev->dev_id);
	}
	//unlock();
}

Device *hal_get(const char *name) {
	lock();
	ListHead *it;
	list_foreach(it, &devices) {
		Device *dev = list_entry(it, Device, list);
		if (strcmp(dev->name, name) == 0) {
			unlock();
			return dev;
		}
	}
	unlock();
	return NULL;
}

static size_t
dev_rw(int type, Device *dev, off_t offset, void *buf, size_t count) {
	DevMessage m;
	//Message m;   
	assert(dev != NULL);
	//assert(sizeof(DevMessage) <= sizeof(Message)); // Message结构体不能定义得太小
	m.header.type = type;
	//m.type = type;
	//-----------------------lab2---------------------
	//m.src = current->PID;
	m.header.src = current->PID;	
	//---------------------------------------------
	m.dev_id = dev->dev_id;
	m.offset = offset;
	m.buf = buf;
	//memcpy(m.payload,buf,510);
	//m.payload = ((char*)buf);
	m.count = count;
	send(dev->pid, (Message*)&m);
	receive(dev->pid, (Message*)&m);
	return m.header.type;
}

size_t
dev_read(Device *dev, off_t offset, void *buf, size_t count) {
	return dev_rw(MSG_DEVRD, dev, offset, buf, count);
}

size_t
dev_write(Device *dev, off_t offset, void *buf, size_t count) {
	//printf("is dev_write work?\n");
	return dev_rw(MSG_DEVWR, dev, offset, buf, count);
}

