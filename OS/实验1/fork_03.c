
#include <unistd.h>
#include <stdio.h>
int main(void)
{
   int i=0;
   printf("i son/pa ppid pid  fpid\n");
   //ppid指当前进程的父进程pid
   //pid指当前进程的pid,
   //fpid指fork返回给当前进程的值
   for(i=0;i<2;i++){
       pid_t fpid=fork();
       if(fpid==0)
    	   printf("%d child  %4d %4d %4d\n",i,getppid(),getpid(),fpid);
       else
    	   printf("%d parent %4d %4d %4d\n",i,getppid(),getpid(),fpid);
   }
   return 0;
}