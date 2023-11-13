#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
sem_t sem,sem1;

void *threadHandler_1(void *arg){
	while(1){
		sem_wait(&sem);
		printf("A");
		sem_post(&sem1);
	}
}

void *threadHandler_2(void *arg){
	while(1){
		sem_wait(&sem1);
		printf("B");
		sem_post(&sem);
	}
}



int main(){

	pthread_t pt1,pt2;
	sem_init(&sem, 0, 1);
	sem_init(&sem1, 0, 0);
	
	pthread_create(&pt1, NULL, threadHandler_1, NULL);
	pthread_create(&pt2, NULL, threadHandler_2, NULL);

	while(1){
		sleep(1);
	}

	return 0;
}


/*

*/
