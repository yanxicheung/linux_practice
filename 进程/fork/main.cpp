#include <unistd.h>
#include <iostream>
using namespace std;

void forkNormal();
void forkZombie();
void forkInit();

typedef void(*Func)() ;

Func demo[] = {forkNormal,forkZombie,forkInit};

void menu()
{	
    printf("****************************\n");
    printf("请选择菜单：\n");
    printf("0:退出 1:forkNormal 2：forkZombie 3：forkInit\n");
}

int main(int argc, char* argv[])
{
    printf("argc = %d\n",argc);
    for(int i = 0;i<argc;i++)
    {
    	printf("argv[%d] = %s\n",i,argv[i]);
    }
    int cmd = 0;
    menu();
    printf("main:pid = %d,ppid = %d\n",getpid(),getppid());
	scanf("%d",&cmd);
	printf("cmd = %d\n",cmd);
	
	switch(cmd)
	{
		case 1:demo[0]();
		case 2:demo[1]();
		case 3:demo[2]();
		default:
			break;
	}    
    printf("main exit\n");
    return 0;
}

void forkNormal()
{
	pid_t pid;
    printf("1:pid:%d,ppid = %d\n",getpid(),getppid());  // 1:pid:6941,ppid = 6913(shell)
    pid = fork();
    if(pid < 0)
    {
        perror("error");
        exit(-1);
    }
    if(pid == 0)
    {
        //子进程执行这个分支;
        printf("2:pid:%d,ppid = %d\n",getpid(),getppid());  //2:pid:6942,ppid = 6941
        pause();
    }
    else
    {
        //父进程执行这个分支;
        // pid == 6942  ---子进程的pid
        // 3:pid:6941,ppid = 6913(shell)
        printf("3:pid:%d,ppid = %d\n",getpid(),getppid()); 
        pause();
    }
    printf("forkNormal exit\n"); 
    exit(0);
}
////////////////////////////////////////////////////////////////

bool isPrimer(int num)
{
    bool isPrimer = true;;
    for(int i = 2;i<num/2;++i) // 如果是质数那么运算的量会大些
    {
        if(num % i == 0)
        {
            isPrimer = false;
            break;
        }
    }
    return isPrimer;
}

void forkZombie()
{
    pid_t pid;
    //  1:pid:7354,ppid = 7326 (shell)
    printf("1:pid:%d,ppid = %d\n",getpid(),getppid()); 
    for(int i = 2;i<7;++i)
    {
        // 2:pid:7354,ppid = 7326 (shell)
        printf("2:pid:%d,ppid = %d\n",getpid(),getppid());
        pid = fork();
        fflush(nullptr);
        if(pid == 0)
        {
            // 3:pid:7355,ppid = 7354
            printf("3:pid:%d,ppid = %d\n",getpid(),getppid()); 
            if(isPrimer(i))
            {
                cout<<"Primer:"<<i<<endl;
            }
            exit(0);
        }
    }
    pause();//父进程没有退出,子进程已经退出造成僵尸进程;
    printf("forkZombie exit\n"); 
    exit(0);
}

//父进程先退出,子进程被init进程接管;
void forkInit()
{
    pid_t pid;
    printf("1:pid:%d,ppid = %d\n",getpid(),getppid());
    for(int i = 2;i<7;++i)
    {
        printf("2:pid:%d,ppid = %d\n",getpid(),getppid());
        pid = fork();
        fflush(nullptr);
        if(pid == 0)
        {
            printf("3:pid:%d,ppid = %d\n",getpid(),getppid());
            if(isPrimer(i))
            {
                cout<<"Primer:"<<i<<endl;
            }
            sleep(1000);
            exit(0);
        }
    }
    printf("forkInit exit\n"); 
    exit(0);
}



