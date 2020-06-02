#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define THREAD_NUM 4
pthread_mutex_t mutex[THREAD_NUM];

static int next(int n)
{
	if(n + 1 == THREAD_NUM)
		return 0;
	return n + 1;
}

static void *print(void *arg)
{
	char i = (char)arg;
    pthread_mutex_lock(&mutex[i]);  //给自己加锁;
	printf("%c\n",i+'a');
	pthread_mutex_unlock(&mutex[next(i)]);//解锁下一个线程;
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
  		pthread_create(tid+i,NULL,print,(void *)i);  // 将(void *)i换成&i有可能造成死锁
	}
	pthread_mutex_unlock(&mutex[0]);
    for(i = 0;i<THREAD_NUM;++i)
	{
  		pthread_join(tid[i],NULL);
  		pthread_mutex_destroy(&mutex[i]);
	}
	exit(0);
}	
