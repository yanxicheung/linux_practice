# 线程的概念

`posix`线程是一个标准，而不是一个实现

线程标识：`pthread_t`

线程的关系是平级的，没有主次之分。

>  ps auwx：查看系统中僵尸进程，僵尸进程的状态会被标注为“Z”。
>
>  ps axf  ：以树形展示进程表
>
>  ps axm ：列出线程，linux下进程线程一致
>
>  ps aux：  列出进程的详细信息
>
> ps  ax -L ：以linux模式来查看线程和进程的关系

将会话看作是一个容器，会话用来承载进程组；

将进程组看作是一个容器，进程组用来承载进程；

将进程看作是一个容器，进程用来承载线程；

调度以线程为单位；

线程和信号的机制最好不要大范围混用。

# 线程的创建

`pthread_create();`

线程的调度取决于调度器的策略。

线程所在的进程退出后，线程也被终止，如下程序创建的线程可能不会被执行。

```c
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
static void * func(void *arg)
{
	puts("thread is working");
	pthread_exit(NULL);
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
```



# 线程的终止

三种方式：

1. 线程从启动例程中返回，返回值为线程的退出码。
2. 线程调用`pthread_exit()`函数;
3. 线程被同一进程中的其他线程取消；

如果调用`pthread_exit()`的线程是当前进程中最后一个正在运行的线程，那么当前进程也会结束。

调用`pthread_exit()`系统会实现线程栈的清理。

`pthread_join()`相当于进程中的`wait`操作，收尸操作。

  创建一个线程默认的状态是joinable(可汇合的), 如果一个线程结束运行但没有被join,则它的状态类似于进程中的Zombie Process（僵尸进程）,即还有一部分资源没有被回收（退出状态码），所以创建线程者应该调用pthread_join来等待线程运行结束，并可得到线程的退出代码，回收其资源（类似于wait,waitpid) 

```c
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
static void * func(void *arg)
{
	puts("thread is working");
	pthread_exit(NULL);
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
	pthread_join(tid,NULL);
	puts("end");
	exit(0);
}
```

## 栈的清理

`pthread_cleanup_push`

`pthread_cleanup_pop`

**以上函数要成对出现，否在编译失败**

```c
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
static void clean(void *arg)  //钩子函数
{
	puts(arg);
}

static void * func(void *arg)
{
	puts("thread is working");

	pthread_cleanup_push(clean,"1");
	pthread_cleanup_push(clean,"2");
	pthread_cleanup_push(clean,"3");
	
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(1);
    pthread_exit(NULL);

//放到exit后面默认会执行
//	pthread_cleanup_pop(0);
//	pthread_cleanup_pop(0);
//	pthread_cleanup_pop(0);
	//return NULL;
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
	pthread_join(tid,NULL);
	puts("end");
	exit(0);
}
```

## 取消选项

`pthread_cancel();`

取消有2中状态：允许和不允许。

允许取消又分为：异步cancel，推迟cancel（默认）--->推迟到cancel点再响应。

取消点：posix定义的取消点，都是可能引发阻塞的系统调用，没有取消点的线程不会被取消。

`pthread_setcancelstate()`：设置是否允许被取消。

`pthread_setcanceltype()`:设置取消方式。

`pthread_testcancel()`:不做任何事，就是一个取消点；



# 线程分离：

`pthread_detach();`

调用pthread_join(pthread_id)后，如果该线程没有运行结束，调用者会被阻塞。

在有些情况下我们并不希望如此，比如在Web服务器中当主线程为每个新来的链接创建一个子线程进行处理的时候，主线程并不希望因为调用pthread_join而阻塞(因为还要继续处理之后到来的链接)。

这时可以在子线程中加入代码pthread_detach(pthread_self()) 。或者父线程调用 pthread_detach(thread_id)(非阻塞，可立即返回)这将该子线程的状态设置为detached(脱离的),则该线程运行结束后会自动释放所有资源。 

# 其他接口

`pthread_equal();`线程标识的比较

`pthread_self();`获得线程的id

# 竞争

以下程序不会正常运行，每次运行结果都不一样：

```c
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#define BEG 30000000
#define END 30000200
#define NUM (END - BEG + 1)

int isPrimer(int num)
{
    int isPrimer = 1;
    for(int i = 2;i<num/2;++i)
    {
        if(num % i == 0)
        {
            isPrimer = 0;
            break;
        }
    }
    return isPrimer;
}
// 代码段是共用的，但是栈每个线程都是独立的。
void *func(void *arg)
{
	int i = *(int*)arg;
	// 每个线程i的地址是不一样的;
	printf("tid = %u,i_addr = %u\n",pthread_self(),&i);
	if(isPrimer(i))
	{
		printf("%d is primer\n",i);
	}
	pthread_exit(NULL);
}

int main(int argc,char *argv[])
{
	int i = 0;
	pthread_t tid[NUM];
	for(i = BEG;i<=END;++i)
	{
	    // 创建的第一线程还没来的及将i的值取走，第二个线程就创建了，此时他们可能获得相同的i值;
  		int ret = pthread_create(tid + i - BEG,NULL,func,&i);
		if(ret != 0)
		{
			fprintf(stderr,"pthread_create failed:%s\n",strerror(ret));
			exit(1);
		}
	}
	for(i = BEG;i<=END;++i)
	{
		pthread_join(tid[i - BEG],NULL);
	}
	exit(0);
}	
```

## 解法1：暴力方案

```c
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define BEG 30000000
#define END 30000200
#define NUM (END - BEG + 1)

int isPrimer(int num)
{
    int isPrimer = 1;
    for(int i = 2;i<num/2;++i)
    {
        if(num % i == 0)
        {
            isPrimer = 0;
            break;
        }
    }
    return isPrimer;
}

void *func(void *arg)
{
	int i = (int)arg;
	printf("tid = %u,i_addr = %u\n",pthread_self(),&i);
	if(isPrimer(i))
	{
		printf("%d is primer\n",i);
	}
	pthread_exit(NULL);
}

int main(int argc,char *argv[])
{
	int i = 0;
	pthread_t tid[NUM];
	for(i = BEG;i<=END;++i)
	{
  		int ret = pthread_create(tid + i - BEG,NULL,func,(void *)i);
		if(ret != 0)
		{
			fprintf(stderr,"pthread_create failed:%s\n",strerror(ret));
			exit(1);
		}
	}
	for(i = BEG;i<=END;++i)
	{
		pthread_join(tid[i - BEG],NULL);
	}
	exit(0);
}	
```

## 解法2：指向不同内存

```c
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define BEG 30000000
#define END 30000200
#define NUM (END - BEG + 1)

int isPrimer(int num)
{
    int isPrimer = 1;
    for(int i = 2;i<num/2;++i)
    {
        if(num % i == 0)
        {
            isPrimer = 0;
            break;
        }
    }
    return isPrimer;
}

void *func(void *arg)
{
	int i = *(int*)arg;
	if(isPrimer(i))
	{
		printf("%d is primer\n",i);
	}
	pthread_exit(arg);
}

int main(int argc,char *argv[])
{
	int i = 0;
	pthread_t tid[NUM];
	for(i = BEG;i<=END;++i)
	{
		int *p = (int *)malloc(sizeof(int));
		*p = i;
  		int ret = pthread_create(tid + i - BEG,NULL,func,p);//这样每个线程拿到的地址就不相同了
		if(ret != 0)
		{
			fprintf(stderr,"pthread_create failed:%s\n",strerror(ret));
			exit(1);
		}
	}
	for(i = BEG;i<=END;++i)
	{
		void *ptr = NULL;
		pthread_join(tid[i - BEG],&ptr);
		free(ptr);
	}
	exit(0);
}	
```

## 解法分析

都不好。

一个进程空间中创建线程的数目是有限的，为了算质数不能一下子创建这个么多线程（线程的栈是独立的）。

ulimit -a：用来显示当前的各种用户进程限制

将线程栈大小改小可以创建更多的线程;



# 互斥量

```c
#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
int g = 10;

void *add(void *arg)
{
	sleep(1);
	g++;
	printf("g = %d\n",g);
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
	exit(0);
}	
```



pthread_mutex_t类型



pthread_mutex_init

pthread_mutex_destroy



pthread_mutex_lock

pthread_mutex_unlock

lock和unlock之间是临接区

# Refrence：

1. [ulimit功能和用法](http://www.360doc.com/content/13/0924/10/14006118_316679434.shtml)