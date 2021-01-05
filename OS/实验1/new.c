//new.c
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
	printf("Child process enter new through exec(), id is %d\n", getpid());
    printf("a=%d\n", a);
    getchar();
    printf("new end\n");
	return 0;
}
