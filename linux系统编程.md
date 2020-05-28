# 段错误

所谓的段错误就是指访问的内存超出了系统所给这个程序的内存空间，**访问到了不该访问的内存**。

Segment fault 之所以能够流行于世，是与glibc库中基本所有的函数都默认型参指针为非空有着密切关系的。

导致段错误的几个直接原因：

（1）**直接解引用一个非法值或NULL的指针**；

​     int *p = nullptr;
​     int a = *p;

（2）在未得到正确的权限时进行访问。例如，试图往一个只读文本段存储值就会引起段错误；

（3）**改写错误： 越过数组边界写入数据**，在动态分配(静态分配的也是)的内存两端之外写入数据，或改写一些堆管理数据结构（在动态分配的内存之前的区域写入数据）；

  p = malloc(256);   p[-1] = 0;   p[256] = 0;

（4）指针释放引起的错误： 

释放同一个内存块两次：

释放一块未曾使用malloc分配的内存：

释放仍在使用中的内存：

释放一个无效的指针：`free((void*)0x5a5a5);`

（5）堆栈越界访问： int a[5]; a[-100000] = 0;

避免方法：

<1>定义了指针后记得初始化，在使用的时候记得判断是否为NULL
<2>在使用数组的时候是否被初始化，数组下标是否越界，数组元素是否存在等
<3>在变量处理的时候变量的格式控制是否合理等

# free:

The free() function frees the memory space pointed to by ptr, which must have been returned by a previous call to malloc(), calloc() or realloc(). Otherwise, or if free(ptr) has already been called before, undefined behavior occurs. **If ptr is NULL, no operation is performed.**



# 数据段、代码段、BSS段、堆栈段

可执行文件中包含数据段（data segment）、代码段（text segment）、BSS段

堆栈段在程序执行时被调用;



# fork和vfork

1 fork:子进程拷贝父进程的数据段、堆栈段; 

   vfork:父子进程共享数据段;

2  vfork并不将父进程的地址空间完全复制给子进程，因为子进程会立即调用exec或exit，也就不会访问该地址空间，只在子进程调用exec之前，它在父进程空间中运行。

3  vfork保证子进程先运行，在它调用exec或exit之后父进程才能调度运行。如果在调用这两个函数之前子进程依赖于父进程的进一步操作，将会导致死锁。



# Linux程序终止

5正常终止

3异常终止





# 进程组

为了统一操作，方便管理；





# wait











# 进程分配

（1）分块

（2）交叉分配

（3）池类分配



# exec函数族







调用fork和exec之前需要刷新需要刷新的缓冲流；





内部命令：cd

外部命令



# 用户权限和组权限

u+s

g+s



# 守护进程

脱离控制终端；

一个会话的leader，一个进程组的leader





# 用宏定义MAX

```c
#include <stdio.h>

#define MAX(a,b)  ((a)>(b)?(a):(b))  //有bug

#define MAX_(a,b) ({typeof(a) A = (a),B = (b); A>B?A:B;})

int max(int a,int b)
{
	return (a>b)?a:b;
}
/*
	max(a,b) = 4
	a = 4,b = 5
	MAX(a,b) = 5
	a = 4,b = 6
	MAX_(a,b) = 1
	a = 4,b = 5
*/
int main()
{
	int a = 3,b = 4;
	printf("max(a,b) = %d\n",max(a++,b++));  // 4
	printf("a = %d,b = %d\n",a,b);
	
	a = 3;
	b = 4;
	printf("MAX(a,b) = %d\n",MAX(a++,b++));
	printf("a = %d,b = %d\n",a,b);
	
    a = 3;
	b = 4;
	printf("MAX_(a,b) = %d\n",MAX_(a++,b++));
	printf("a = %d,b = %d\n",a,b);
	return 0;
}
```



```makefile
SOURCE = ${wildcard *.c}
OBJS = ${SOURCE:.c=.o}
TARGET=main.out
CC=gcc
CFLAGS=-g -Wall -c

${TARGET}:${OBJS}
	${CC} $^ -o $@
	
${OBJS}:%.o:%.c
	${CC} ${CFLAGS} $< -o $@

.PHONY:clean
clean:
	${RM} *.o *.out -r
```

# 进程终止方式



# 钩子函数



# 命令行分析

getopt()

getopt_long()



# 环境变量

查看环境变量：export

在将操作系统当作一个运行的程序时，可以将环境变量看作是全局变量。

重要的环境变量：PATH

```c
#include <stdio.h>
#include <stdlib.h>
//打印所有环境变量
extern char** environ;
int main()
{
    int i = 0;
    for(i = 0;environ[i]!=NULL;++i)
    {
        puts(environ[i]);
    }
    exit(0);
}
```

## 高频接口：

getenv();

setenv();

## C语言程序空间：



# 并发的两种方式：

## 信号

**信号会打断阻塞的系统调用**，该系统调用会返回失败，通过判断errno可以知道是否是被信号打断;

```c
// 原始写法;
sfd = open();
if(sfd < 0)
{
    if(errno != EINTR)
    {
        perror("open failed");
        exit(1);
    }
}
//考虑阻塞的系统调用被信号打断;
do
{
    sfd = open();
    if(sfd < 0)
    {
        if(errno != EINTR)
        {
            perror("open failed");
            exit(1);
        }
    }
    
}while(sfd<0);
```





### 信号的不可靠

标准信号会丢失，实时信号不会丢失

信号的行为不可靠：

（1）handler不是人为调用，handler的执行现场由内核布置

（2）一个信号在处理的同时，又来了一个相同的信号，由于执行现场是内核布置的，第二次的执行现场可能把第一次的冲掉了。



```c
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
// 程序退出了，信号就执行不了了;
void handler(int s)
{
	write(STDOUT_FILENO,"!",1);
}
int main()
{

	int i;
//	signal(SIGINT,SIG_IGN);

	signal(SIGINT,handler);
	for(i = 0;i<10;++i)
	{
		write(STDOUT_FILENO,"*",1);
		sleep(1);
	}
	exit(0);
}
```



### 可重入函数



### 信号的响应过程



### 常用函数

signal

kill ：发送信号

raise：

alarm：

pause：

abort：

system：

sleep：

### 信号集



### 信号屏蔽字和pending集的处理



### 扩展

sigsuspend

signaction

setitimer



信号处理函数中不要使用不可重复的函数，不要操作IO，不要处理耗时长的任务。



# Refrence：

1. [c语言中常见的几个段错误](https://blog.csdn.net/lvguangj/article/details/8099447)
2. [数据段、代码段、BSS段、堆栈段](https://blog.csdn.net/weixin_38233274/article/details/80321719)
3. [C/C++段错误问题排查和解决方法](https://blog.csdn.net/chenyulancn/article/details/51693128?utm_medium=distribute.pc_relevant.none-task-blog-baidujs-3)
4. [Linux下进程终止过程](https://www.cnblogs.com/brucemengbm/p/7002836.html)

