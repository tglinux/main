#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include "contacts.h"
T_CONT* phead = NULL;	//定义头指针
/* 函数声明 */
int displayMenu();
/* 信号SIGNAL处理函数 [void (*sighandler_t)(int)] */
void _SIGINT_handler(int signum) {
		linkListDestory(phead);
		printf("Catch siganl : %d\n", signum);
		_exit(0);
}

/* 主函数 */
int main(){

	int choose;

	if( !linkListInit(&phead) ){	//链表初始化
		fprintf(stderr,"#Error : linklist init failed.\n");
		return -1;
	}

	while(1){
		choose = displayMenu();	//获得下一步指示
		switch( choose ){
			case 1 : printNode(phead);break;
			case 2 : insertNode(phead);break;
			case 3 : deleteNode(phead);break;
			case 4 : searchNode(phead);break;
			case 5 : linkListDestory(phead);return 0;
			default : {
				printf("### Invalid option argument.\n");
				printf("### Press Enter to continue.\n");
				getchar();
				break;
			}
		
		}
		signal(SIGINT, _SIGINT_handler); 		//试图去捕获 SIGINT 并执行 _SIGNAL_handler()
	}

	return 0;
}

/* 显示菜单 */
int displayMenu(){
	int choose;
	system("clear");	//清屏
	printf(" ____________________________\n");
	printf("|                            |\n");
	printf("|  1 - Display Contacts      |\n");
	printf("|  2 - Create a new contact  |\n");
	printf("|  3 - Delete a contact      |\n");
	printf("|  4 - Search contact        |\n");
	printf("|  5 - Exit                  |\n");
	printf("|____________________________|\n");
	printf("Input choose : ");
	scanf("%d%*c",&choose);
	return choose;
}


/*

栈上的内存,生命周期在定义时开始,结束在使用完成,内存由系统释放
堆上的内存,生命周期在malloc开始,结束在free释放

	void *malloc(size_t size);
	函数功能:申请动态内存
	函数参数:size	   	 要申请的内存大小(bytes)
	返 回 值:成功返回指针,指向已经分配好的内存;失败,返回NULL

	void free(void *ptr);
	函数功能:释放动态内存
	函数参数:ptr		 指向要释放的内存的首地址
	返 回 值:无返回值

	//在堆上申请 4 字节的内存 用于存放一个整型值
	int *p = (int *)malloc(sizeof(int));
	if( p == NULL ){
		perror("malloc");
		return -1;
	}
	*p = 123;
	printf("*p = %d\n",*p);
	free(p);
*/
