//exp01_wait.c
#include<stdio.h> 
#include<stdlib.h> 
#include<sys/wait.h> 
#include<unistd.h> 

int main() 
{
    if (fork()== 0)
    {
    	printf ("Child process begin, id is %d\n", getpid());
    	printf ("Child process end\n");
        exit(0);         /* 子进程终止 */
        
    }
    else
    {
    	printf ("Parent process begin waiting, parent id is %d\n", getpid());
        int exit_pid = wait(NULL); /* 父进程等待子进程终止 */
        printf ("Parent process end, exit child id is %d\n", exit_pid);
    }
    return 0; 
} 