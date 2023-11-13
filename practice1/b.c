#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


key_t key_value;	//键值
int   msqid;		//消息队列标识符
struct msgbuf msg;

/* 消息结构 msgbuf */
struct msgbuf {
	long mtype;				/* 消息类型(非负) */
	char data[1024];		/* 消息数据(用户自定义) */
};
void* msg_snd(void* arg)
{
	while (1) {
		fgets(msg.data, sizeof(msg.data), stdin);
		msg.mtype = 1235;
		msgsnd(msqid, &msg, sizeof(msg) - sizeof(long), 0);

	}
}
//void* msg_rcv(void* arg)
//{
//	while (1) {
//		msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 1234, 0);
//
//		printf("recv : %s\n", msg.data);
//	}
//}


/* 主函数 */
int main(int argc, char** argv) {

	pthread_t pt;
	/* 创建键值 */
	if (-1 == (key_value = ftok("/home", 123))) {
		perror("ftok");
		return -1;
	}
	/* 获取一个消息队列标识符 */
	if (-1 == (msqid = msgget(key_value, IPC_CREAT | 0666))) {
		perror("msgget");
		return -1;
	}
	if (pthread_create(&pt, NULL, msg_snd, NULL)) {
		perror("pthread_create");
		return -1;
	}
	while (1) {
		if (-1 == msgrcv(msqid, &msg, sizeof(msg) - sizeof(long), 1234, 0)) {
			perror("msgrcv");
			return -1;
		}
		printf("recv : %s\n", msg.data);
	}



	return 0;
}