#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
	printf("1:pid = %d,ppid = %d\n",getpid(),getppid());
	for(int i = 0;i<5;++i)
	{
	    pid_t pid = fork();
		if(pid == 0)  //子进程运行完后退出;
		{
			printf("2:pid = %d,ppid = %d\n",getpid(),getppid());
			exit(0);
		}
	}	
	
	int status;
	for(int i = 0;i<5;++i)  //父进程等待,避免僵尸进程;
	{
		wait(&status);
		if(WIFEXITED(status)) // 子进程退出成功;  if the child terminated normally
		{
			printf("exit status[%d]\n",WEXITSTATUS(status));
		}
		else // 失败
		{
			printf("failed status[%d]\n",WEXITSTATUS(status));
		}
	}
	return 0;
}
