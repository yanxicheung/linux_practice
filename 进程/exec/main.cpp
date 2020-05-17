#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
/*
打印时戳
date + %s

exec之前需要刷新流;
*/
int main_exec(int argc,char *argv[])
{
	printf("1:pid = %d,ppid = %d\n",getpid(),getppid());
	puts("beg");
	fflush(NULL); // 没有fflush  ./exec.out > /tmp/out时beg不会输出到文件;
	execl("/bin/date","date","+%s",NULL);
	perror("execl failed");
	puts("end");
	exit(0);
}


int main_fews(int argc,char *argv[])
{
	puts("beg");
	fflush(NULL);
	pid_t pid = fork();
	if(pid < 0)
	{
		perror("fork err");
		exit(1);
	}
	
	if(pid == 0)
	{
		execl("/bin/date","date","+%s",NULL);
		perror("exec error");
		exit(1);
	}
	
    wait(NULL);
	puts("end");
	exit(0);
}


strsep
strtoks
glob



