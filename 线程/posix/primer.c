#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int g = 10;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *add(void *arg)
{
	sleep(1);
	pthread_mutex_lock(&mutex);
	g++;
	printf("g = %d\n",g);
	pthread_mutex_unlock(&mutex);
	pthread_exit(NULL);
}

int main(int argc,char *argv[])
{
	unsigned int i = 0;
	pthread_t tid[20];
	for(i = 0;i<20;++i)
	{
  		pthread_create(tid+i,NULL,add,NULL);
	}
	
    for(i = 0;i<20;++i)
	{
  		pthread_join(tid[i],NULL);
	}
	pthread_mutex_destroy(&mutex);
	exit(0);
}	
