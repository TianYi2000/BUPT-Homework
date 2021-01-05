#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <signal.h>

void func();

int main()
{
	int cid;
	signal(12,func);	//自定义信号12为运行func
	if(cid=fork())
	{

		printf("Parent send a signal of 'funcid' to child.\n");	
		sleep(0.1);
		kill(cid,12);	//先发送信号12，自定义信号
		sleep(0.1);
		kill(cid,9);	//再发送信号9，kill子进程
		wait(0);
		printf("Child process end.\n");
		printf("Parent process end.\n");
	}
	else
	{
		/* 子进程 */
		printf("Child receive a signal from parent.\n");
		while (1)
		{
			printf("Child Running...\n");
			sleep(0.1);
		}
		exit(0);
	}
	return 0;
}

void func()
{
	printf("child process receive 12\n");
}
