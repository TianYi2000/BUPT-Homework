//exp01_exec.c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
int a;
int main()
{
	if(fork())
	{
		printf("This is parent process, id is %d.\n", getpid());
		wait(NULL);
	}
	else
	{
        a = 123;
		printf("This is child process, id is %d.\n", getpid());
		getchar();
		if(execl("./new","./new",NULL)<0)
			perror("cannot exec new");
	}
	while(1);
	printf("End\n");
	return 0;
}
