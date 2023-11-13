#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

/*
	1.使用线程进行全双工通信
	2.当连接被服务端关闭时,打印"Connection closed by foreign host."
*/

/* 主函数 */
int main(int argc, char **argv){

	/* 要连接的服务端IP和端口号由主函数参数传入 */
	if( argc != 3 ){
		printf("<usage> : %s [ipv4] [port]\n", argv[0]);
		return -1;
	}

	int client_socket;			//客户端套接字
	pthread_t pt;

	struct sockaddr_in target;	//目标的internet协议地址结构
	char buffer[1024] = {0};

	/* 1.初始化套接字 */
	if( -1 == (client_socket = socket(AF_INET, SOCK_STREAM, 0)) ){
		perror("socket");
		return -1;
	}

	/* 2.连接服务端 */
	memset(&target,0x00,sizeof(target));
	target.sin_family      = AF_INET;
	target.sin_port        = htons(atoi(argv[2]));
	target.sin_addr.s_addr = inet_addr(argv[1]);
	if( connect(client_socket, (const struct sockaddr *)&target, sizeof(target)) ){
		perror("connect");
		return -1;
	}

	/* 3.数据传输 */
	while(1){
		if( read(client_socket, buffer, sizeof(buffer)) < 0 ){
			perror("read");
			break;
		}
		printf("recv : %s\n", buffer);
		memset(buffer, 0x00, sizeof(buffer));

	}

	close(client_socket);
	
	return 0;
}

/*
	int socket(int domain, int type, int protocol);
 	初始化套接字
 		@domain		协议族
 		@type		套接字类型
 		@protocol	如果套接字设置没有由前两个参数完全指定,那么次参数再做设置
 					如果前两个参数已经填写好,那么此参数写0

====================================================================================
 	int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
	客户端向服务端发起连接请求(协议族/地址族 , IP地址 , 端口号)
		@sockfd		客户端套接字
		@addr		结构体指针,连接时将从这个指针指向的区域中寻找目标IP端口号以及协议族
		@addrlen	协议地址结构长度(字节)

====================================================================================
	in_addr_t inet_addr(const char *cp);
	将点分十进制形式的IPv4地址转换为无符号32位网络字节序的数值


通用地址结构
struct sockaddr{
	u_short sa_family; 	// 地址族 , AF_xxx
	char sa_data[14]; 	// 14 字节协议地址
};

Internet 协议地址结构
struct sockaddr_in{
	u_short sin_family; 		// 地址族，AF_INET，2bytes
	u_short sin_port; 			// 端口，2bytes
	struct in_addr sin_addr; 	// IPV4 地址，4bytes
	char sin_zero[8]; 			// 8bytes unused作为填充
};

IPv4 地址结构
struct in_addr{
	in_addr_t s_addr; 	//u32 network address
};


*/
