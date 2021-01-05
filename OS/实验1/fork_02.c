#include <unistd.h>
#include <stdio.h> 
int main () 
{ 
	pid_t fpid;
	int count=0, i;
	fpid=fork(); 
	if (fpid < 0) 
		printf("error in fork!"); 
	else if (fpid == 0) {
        //getpid()获取当前进程id
        for (i = 1 ; i <= 100 ; ++i)
        {
            printf("i am the child process, my process id is %d\n",getpid()); 
			count++;
        }
		
	}
	else {
        //getpid()获取当前进程id
		for (i = 1 ; i <= 100 ; ++i)
        {
            printf("i am the parent process, my process id is %d\n",getpid()); 
			count++;
        }
	}
	printf("统计结果是: %d\n",count);
	return 0;
}