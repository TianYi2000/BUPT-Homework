//sample.c
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <wait.h>
#include <signal.h>
#define ORANGE_MAX_VALUE 1000000
#define APPLE_MAX_VALUE 100000000
#define MSECOND 1000000
struct apple
{
	unsigned long long a;
	unsigned long long b;
};
struct apple_cache128
{
    unsigned long long a;
	char c[128];  /*32,64,128*/
	unsigned long long b;
	
};
struct apple_cache64
{
    unsigned long long a;
	char c[64];  /*32,64,128*/
	unsigned long long b;
	
};
struct apple_cache32
{
    unsigned long long a;
	char c[32];  /*32,64,128*/
	unsigned long long b;
	
};
struct orange
{
	int a [ORANGE_MAX_VALUE];
	int b [ORANGE_MAX_VALUE];
};
int state;

int K = 10;
pthread_mutex_t mutex;

void* CalApple(void * arg)
{

	
	if (state == 1)
	{
		struct apple* Apple = (struct apple *) arg;
		for(long long sum=0; sum<APPLE_MAX_VALUE ; sum++)
		{
			Apple->a += sum;
			Apple->b += sum;
		}
	}
	else
	{
		struct apple* Apple = (struct apple *) arg;
		for(int sum=0; sum<APPLE_MAX_VALUE ; sum++)
		{
			Apple->a += sum;
			Apple->b += sum;
		}
	}
	
}
void* CalOrange(void * arg)
{
	struct orange* Orange = (struct orange *) arg;
		int sum = 0;
		for(int i=0;i<ORANGE_MAX_VALUE; i++)
		{
			sum += Orange->a[i]+Orange->b[i];
		}

	
}

void* addx_lock(void* x)
{
	pthread_mutex_lock(&mutex);
	for(int sum=0;sum<APPLE_MAX_VALUE;sum++)
	{
		((struct apple *)x)->a += sum;
	}
	pthread_mutex_unlock(&mutex);
}
 
void* addy_lock(void* y)
{
	pthread_mutex_lock(&mutex);
	for(int sum=0;sum<APPLE_MAX_VALUE;sum++)
	{
		((struct apple *)y)->b += sum;
	}
	pthread_mutex_unlock(&mutex);
	
}

void* addx(void* x)
{
	for(int sum=0;sum<APPLE_MAX_VALUE;sum++)
	{
		((struct apple *)x)->a += sum;
	}
}
 
void* addy(void* y)
{
	for(int sum=0;sum<APPLE_MAX_VALUE;sum++)
	{
		((struct apple *)y)->b += sum;
	}
	
}

void Run1()
{
	struct apple Apple;
	struct orange Orange;
	CalApple(&Apple);
	CalOrange(&Orange);
}

void Run2()
{
	struct apple Apple;
	struct orange Orange;
	pthread_t ThreadA;
		
	pthread_create(&ThreadA,NULL,CalOrange,&Orange);
	CalApple(&Apple);
    pthread_join(ThreadA,NULL);
}

void Run3_lock()
{
	struct apple Apple;
	struct orange Orange;

	pthread_mutex_init(&mutex,NULL);

	pthread_t ThreadA,ThreadB;
	
	pthread_create(&ThreadA,NULL,addx_lock,&Apple);
	pthread_create(&ThreadB,NULL,addy_lock,&Apple);
 	CalOrange(&Orange);
	
	pthread_join(ThreadA,NULL);
	pthread_join(ThreadB,NULL);

	pthread_mutex_destroy(&mutex);
}

void Run3_no_lock()
{
	struct apple Apple;
	struct orange Orange;

	pthread_t ThreadA,ThreadB;
	
	pthread_create(&ThreadA,NULL,addx,&Apple);
	pthread_create(&ThreadB,NULL,addy,&Apple);
 	CalOrange(&Orange);

	pthread_join(ThreadA,NULL);
	pthread_join(ThreadB,NULL);
}

void Run3_cache_128()
{
	struct apple_cache128 Apple;
	struct orange Orange;

	pthread_t ThreadA,ThreadB;
	
	pthread_create(&ThreadA,NULL,addx,&Apple);
	pthread_create(&ThreadB,NULL,addy,&Apple);
 	CalOrange(&Orange);

	pthread_join(ThreadA,NULL);
	pthread_join(ThreadB,NULL);
}

void Run3_cache_64()
{
	struct apple_cache64 Apple;
	struct orange Orange;

	pthread_t ThreadA,ThreadB;
	
	pthread_create(&ThreadA,NULL,addx,&Apple);
	pthread_create(&ThreadB,NULL,addy,&Apple);
 	CalOrange(&Orange);

	pthread_join(ThreadA,NULL);
	pthread_join(ThreadB,NULL);
}

void Run3_cache_32()
{
	struct apple_cache32 Apple;
	struct orange Orange;

	pthread_t ThreadA,ThreadB;
	
	pthread_create(&ThreadA,NULL,addx,&Apple);
	pthread_create(&ThreadB,NULL,addy,&Apple);
 	CalOrange(&Orange);

	pthread_join(ThreadA,NULL);
	pthread_join(ThreadB,NULL);
}

int main (int argc, const char * argv[]) {
	double time;

	printf("1 thread\n");
	time = 0;state = 1;
	for (int i = 1; i <= K; ++i)
	{
		
		struct timeval start;
    	struct timeval end;
		gettimeofday(&start,NULL);
		Run1();	
		gettimeofday(&end,NULL);
		double RunTime =(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
		printf("%dth run, time = %.4lfs\n", i, RunTime);
		time += RunTime;
		
	}
	printf("Avg Time = %.4lfs\n\n", time / K);

	printf("2 thread\n");
	time = 0;state = 2;
	for (int i = 1; i <= K; ++i)
	{
		
		struct timeval start;
    	struct timeval end;
		gettimeofday(&start,NULL);
		Run2();	
		gettimeofday(&end,NULL);
		double RunTime =(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
		printf("%dth run, time = %.4lfs\n", i, RunTime);
		time += RunTime;
		
	}
	printf("Avg Time = %.4lfs\n", time / K);

	printf("3 thread with lock\n");
	time = 0;state = 3;
	for (int i = 1; i <= K; ++i)
	{
		
		struct timeval start;
    	struct timeval end;
		gettimeofday(&start,NULL);
		Run3_lock();	
		gettimeofday(&end,NULL);
		double RunTime =(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
		printf("%dth run, time = %.4lfs\n", i, RunTime);
		time += RunTime;
		
	}
	printf("Avg Time = %.4lfs\n", time / K);

	printf("3 thread with no lock\n");
	time = 0;state = 4;
	for (int i = 1; i <= K; ++i)
	{
		
		struct timeval start;
    	struct timeval end;
		gettimeofday(&start,NULL);
		Run3_no_lock();	
		gettimeofday(&end,NULL);
		double RunTime =(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
		printf("%dth run, time = %.4lfs\n", i, RunTime);
		time += RunTime;
		
	}
	printf("Avg Time = %.4lfs\n", time / K);

	printf("3 thread with no lock, cache = 128\n");
	time = 0;state = 5;
	for (int i = 1; i <= K; ++i)
	{
		
		struct timeval start;
    	struct timeval end;
		gettimeofday(&start,NULL);
		Run3_cache_128();	
		gettimeofday(&end,NULL);
		double RunTime =(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
		printf("%dth run, time = %.4lfs\n", i, RunTime);
		time += RunTime;
		
	}
	printf("Avg Time = %.4lfs\n", time / K);

	printf("3 thread with no lock, cache = 64\n");
	time = 0;state = 6;
	for (int i = 1; i <= K; ++i)
	{
		
		struct timeval start;
    	struct timeval end;
		gettimeofday(&start,NULL);
		Run3_cache_64();	
		gettimeofday(&end,NULL);
		double RunTime =(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
		printf("%dth run, time = %.4lfs\n", i, RunTime);
		time += RunTime;
		
	}
	printf("Avg Time = %.4lfs\n", time / K);

	printf("3 thread with no lock, cache = 32\n");
	time = 0;state = 7;
	for (int i = 1; i <= K; ++i)
	{
		
		struct timeval start;
    	struct timeval end;
		gettimeofday(&start,NULL);
		Run3_cache_32();	
		gettimeofday(&end,NULL);
		double RunTime =(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
		printf("%dth run, time = %.4lfs\n", i, RunTime);
		time += RunTime;
		
	}
	printf("Avg Time = %.4lfs\n", time / K);

	return 0;
}