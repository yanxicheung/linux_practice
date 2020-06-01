#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define THREAD_NUM 4
pthread_mutex_t mutex[THREAD_NUM];

void *print(void *arg)
{
	char i = (char)arg;
    pthread_mutex_lock(&mutex[i]);
	printf("%c\n",i+'a');
	pthread_mutex_unlock(&mutex[(i+1)%THREAD_NUM]);
	pthread_exit(NULL);
}

int main(int argc,char *argv[])
{
	unsigned int i = 0;
	pthread_t tid[THREAD_NUM];
	
	for(i = 0;i<THREAD_NUM;++i)
	{
		pthread_mutex_init(&mutex[i],NULL);
		pthread_mutex_lock(&mutex[i]);
  		pthread_create(tid+i,NULL,print,(void*)i);
	}
	pthread_mutex_unlock(&mutex[0]);
    for(i = 0;i<THREAD_NUM;++i)
	{
  		pthread_join(tid[i],NULL);
  		pthread_mutex_destroy(&mutex[i]);
	}
	
	exit(0);
}	
