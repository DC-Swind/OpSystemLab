#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#define ANY -1
#define MSG_SZ 512
#include "common.h"
#include "list.h"
#define MAILBOX_SIZE 1024
typedef uint32_t pid_t;
typedef uint32_t off_t;
struct Message {
    int type; // 消息的类型
    pid_t src, dest; // 消息发送者和接收者的pid
    char payload[MSG_SZ];
    ListHead list;
};
typedef struct Message Message;
struct MsgHead {
    int type;
    pid_t src,dest;
    //char payload[MSG_SZ];
};
typedef struct MsgHead MsgHead;
void send(pid_t dst, Message *m);
void receive(pid_t dst, Message *m);

void init_mailbox();
#endif
