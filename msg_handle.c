#include "msg_handle.h"
#include "fds_manage.h"
void msgHandler(int srcfd, char *msg){
	char* error_buffer = "please write again\n";
	char* p_msg = NULL;
	//p_msg = msg;
	while (*msg == '\r' || *msg == '\n' || *msg == ' ') {
		msg++;
	}
	if (*msg == '\0') {
		if (srcfd == STDOUT_FILENO) {
			
			 write(srcfd, error_buffer, strlen(error_buffer));
			 send(srcfd, error_buffer, strlen(error_buffer), 0);
		}
		else {
			send(srcfd, error_buffer, strlen(error_buffer), 0);
			
		}
	
	}else {
		msgBroadcast(srcfd, msg);
	}
	
	
	
}

void msgBroadcast(int srcfd, char *msg){
	fd_set fds;
	int i;
	copyFds(&fds); 
	for(i = 0; i < FD_SETSIZE; i++){
		if( FD_ISSET(i, &fds) ){
			sendMsgToTarget(srcfd, i, msg);
		}
	}
}

ssize_t sendMsgToTarget(int srcfd, int destfd, char *msg){
	char buffer[1024] = { 0 };
	sprintf(buffer ,"client%03d : %s\n", srcfd, msg);
	if( destfd == STDOUT_FILENO ){
		return write(destfd, buffer, strlen(buffer));
	}else{
		return send(destfd, buffer, strlen(buffer), 0);
	}
}

