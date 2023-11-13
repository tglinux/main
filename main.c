#include "server.h"
#include "client_handle.h"
#include "msg_handle.h"
extern char* optarg;
extern int optind, opterr, optopt;
int limit;
int main(int argc, char **argv){

	unsigned short port = 60000;
	int server_socket;
	pthread_t pt;
	char buffer[1024]  = {0};
	char *p_sktid = NULL;
	char *p_msg = NULL;
	int opt;
	limit = 10;
	
	while ((opt = getopt(argc, argv, "p::l::")) != -1) {
		switch (opt) {
		case 'p':port = atoi(optarg); break;
		case 'l':limit = atoi(optarg); break;
		default:
			return -1;
		}
	}
	printf("limit = %d\n", limit);

	if( -1 == (server_socket = serverSocketInit(port)) ){
		printf("#Error : Server socket initialize failed.\n"); return -1;
	}
	if( pthread_create(&pt, NULL, threadAcceptClient, &server_socket) ){
		perror("pthread_create");
		return -1;
	}

	while (1) {
		memset(buffer, 0x00, sizeof(buffer));
		fgets(buffer, sizeof(buffer), stdin);

		if (*buffer == '/') {
			for (int i = 0; i <= FD_SETSIZE; i++) {
				p_sktid = buffer + 1;	//指向id编号的字符
				p_msg = buffer;
				while (*p_msg != ':') {
					p_msg++;
				}
				if (atoi(p_sktid) == i) {
					if (send(atoi(p_sktid), p_msg + 1, strlen(p_msg + 1), 0) < 0) {
						perror("send");
					}
				}

			}
			
		}
			msgHandler(STDOUT_FILENO, buffer);
		}	
		
	return 0;
}



/*
	int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
	用来接收客户端的连接
		@sockfd  - 服务端套接字
		@addr    - 协议地址结构指针,如果需要存储连接的客户端协议地址信息
				   使用该指针指定存储区域首地址
		@addrlen - 指针,该指针指向的内存单元中存储了协议地址信息的大小
	成功返回一个套接字,该套接字用来和刚刚连接好的客户端进行通信
	失败返回-1

*/
