#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define BEG 30000000
#define END 30000200

#define THREAD_NUM    4

unsigned int num = 0;

pthread_mutex_t mutex_num = PTHREAD_MUTEX_INITIALIZER;

int isPrimer(int num)
{
    int isPrimer = 1;;
    for(int i = 2;i<num/2;++i) // 如果是质数那么运算的量会大些
    {
        if(num % i == 0)
        {
            isPrimer = 0;
            break;
        }
    }
    return isPrimer;
}

static void *calcPrimer(void *arg)
{
	int id = (int)arg;
	while(1)
	{
	    pthread_mutex_lock(&mutex_num);  //给自己加锁;
	    printf("id = %d\n",id);
		if(isPrimer(num))
		{
			printf("%d\n",num);
		}
		pthread_mutex_unlock(&mutex_num);
		if(num == -1)
		{
		   pthread_exit(NULL);
		}
	}
}

int main(int argc,char *argv[])
{
	unsigned int i = 0;
	pthread_t tid[THREAD_NUM];
	pthread_mutex_lock(&mutex_num);
	for(i = 0;i < THREAD_NUM;++i)
	{
  		pthread_create(tid+i,NULL,calcPrimer,(void *)i);
	}
	
	for(i = BEG ;i < END;++i)
	{
		num = i;
	    pthread_mutex_unlock(&mutex_num);
	}
	num = -1;


    for(i = 0;i<THREAD_NUM;++i)
	{
  		pthread_join(tid[i],NULL);
  		pthread_mutex_destroy(&mutex_num);
	}
	exit(0);
}	
