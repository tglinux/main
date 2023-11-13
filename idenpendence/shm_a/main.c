#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include "mysem.h"

#define PAGE_SIZE	4096

int main(int argc, char **argv){

	key_t key_value;		//键值
	int   shmid;			//共享内存标识符
	int   semid;
	void *p_shm = NULL;		//共享内存段的首地址

	/* 获取并初始化信号量 */
	semid = semArrInit(2);
	if( -1 == (key_value = ftok("/home",222)) ){
		perror("ftok");
		return -1;
	}
	/* 获取共享内存标识符 */
	if( -1 == (shmid = shmget(key_value,PAGE_SIZE, IPC_CREAT | 0666)) ){
		perror("shmget");
		return -1;
	}

	/* 接入目标共享内存段 获取内存的首地址 */
	if( (void *)-1 == (p_shm = shmat(shmid, NULL, 0)) ){
		perror("shmat");
		return -1;
	}
	printf("=== Attached shared memory succeed ===\n\n");

	char buffer[1024] = {0};
	/* 数据写 */
	while(1){
		/* 申请信号量资源(-1) */
		semP(semid, 0);
		fgets(buffer, sizeof(buffer), stdin);
		memcpy(p_shm, buffer, sizeof(buffer));
		/* 释放信号量资源(+1) */
		semV(semid, 1);
		/* 释放信号量资源(+1) */
		semV(semid, 0);
	}

		

	/* 与目标共享内存段断开连接 */
	if( -1 == shmdt(p_shm) ){
		perror("shmdt");
		return -1;
	}
	printf("\n=== Detached shared memory succeed ===\n");

	return 0;
}

/*
	int shmget(key_t key, size_t size, int shmflg);
	函数功能:获取一段共享内存标识符
	函数参数:key		 键值
			 size	 共享内存段的大小(bytes) 必须是PAGE_SIZE的倍数
			 shmflg	 标志位 - IPC_CREAT + 权限参数
	返 回 值:成功返回一个有效的共享内存标识符
			 失败返回-1


	[shared memory attach - 接入共享内存]
	WWshmat(int shmid, const void *shmaddr, int shmflg);
	函数参数:shmid			 共享内存段标识符
			 shmaddr	 写NULL - 让系统自动分配地址来接入共享内存( suitable unused address )
			 shmflg      一般写0
	返 回 值:成功返回接入的共享内存段的首地址,失败返回(void *)-1

	[shared memory detach - 断开共享内存连接]
	int shmdt(const void *shmaddr);
	函数参数:shmaddr		要断开的共享内存段的首地址
	返 回 值:成功返回0,失败返回-1
	

*/
