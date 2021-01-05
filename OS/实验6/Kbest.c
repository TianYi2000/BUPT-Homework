//exp01_exec.c
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>


int main(int argc, char * argv[])
{
	for (int i = 1; i <= 10; ++i) T[i] = 999;
	char* str;
	str = (char *) malloc(20);
	strcpy(str, "./");
	strcpy(str + 2, argv[1]);

	printf("%s\n", str);
	
	for (int i = 1; i <= 200; ++i)
	{
		printf("%dth run\n", i);
		struct timeval start;
    	struct timeval end;
		gettimeofday(&start,NULL); 
		if(execl(str,str,NULL)<0)
			perror("cannot exec");		
		gettimeofday(&end,NULL);
		if(update((end.tv_usec - start.tv_usec) / 1000000.0))
		{
			printf("Non convergence!\n");
			// return 0;
		}
		
	}
	
	for (int i = 1; i <= 10; ++i)
		printf("V[%d] = %.3lfs\n",i,T[i] );
	printf("Avg Time = %.3lfs\n", avg);
}
