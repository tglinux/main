#include "client_handle.h"

/* 全局变量 */
sem_t sem;

/* 	threadAcceptClient - 线程处理函数 - 接收客户端连接
	@arg - 指向主线程监听套接字首地址								*/
void *threadAcceptClient(void *arg){
	int sockfd = *(int *)arg;		/* 服务端监听套接字 */
	int new_client;					/* 用来存储接受的客户端套接字 */
	pthread_t pt;

	sem_init(&sem, 0, 1);

	while(1){
		sem_wait(&sem);
		if( -1 == (new_client = accept(sockfd, NULL, NULL)) ){
			perror("accept");
			break;
		}else{
			if( pthread_create(&pt, NULL, threadHandleClient, &new_client) ){
				perror("pthread_create");
				close(new_client);
			}
		}
	}
	_exit(-1);
}


/* threadHandleClient - 线程处理函数 - 接收已连接客户端发送的消息
   @arg - 指向新连接至服务端的客户端套接字首地址	*/
void *threadHandleClient(void *arg){
	int client_socket = *(int *)arg;
	char buffer[1024] = {0};
	ssize_t len;
	struct sockaddr_in client_addr;	/* 用来存储接收的客户端协议地址信息 */
	socklen_t addr_len = sizeof(client_addr);
	sem_post(&sem);

	getpeername(client_socket, (struct sockaddr *)&client_addr, &addr_len);
	printf("==== client%02d %-15s : %d connected.  ====\n" \
	,client_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port) );
	
	while (1){
		memset(buffer, 0x00, sizeof(buffer));
		len = recv(client_socket, buffer, sizeof(buffer), 0);
		if( len > 0 ){
			printf("client%02d : %s\n", client_socket, buffer);
		}else if(!len) {
			printf("#### client%02d %-15s : %d disconnected ####\n" \
				,client_socket, inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
			break;
		}else{perror("recv");break;}
	}
	close(client_socket);
	pthread_detach(pthread_self());
	pthread_exit(NULL);
}

