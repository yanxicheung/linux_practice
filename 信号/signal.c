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
