#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

void *threadHandler_1(void *arg){
	while(1){
		printf("A");
	}
}

void *threadHandler_2(void *arg){
	while(1){
		printf("B");
	}
}


int main(){

	pthread_t pt1,pt2;
	
	pthread_create(&pt1, NULL, threadHandler_1, NULL);
	pthread_create(&pt2, NULL, threadHandler_2, NULL);

	while(1){
		sleep(1);
	}

	return 0;
}


/*

*/
