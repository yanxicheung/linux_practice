#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
static void * func(void *arg)
{
	puts("thread is working");
	return NULL;
}

int main()
{
	puts("beg");
	pthread_t tid;
	int err = pthread_create(&tid,NULL,func,NULL);
	if(err!=0)
	{
		fprintf(stderr,"pthread_create:%s\n",strerror(err));
		exit(1);
	}
	puts("end");
	exit(0);
}
