//CPU2.c
#define _GNU_SOURCE
#include<sched.h>
#include<stdio.h>
#include<pthread.h>
#include<sys/time.h>
#include<unistd.h>
#include<sys/syscall.h>
pid_t gettid()
{
	return syscall(SYS_gettid);
}

#define ORANGE_MAX_VALUE 1000000
#define APPLE_MAX_VALUE 100000000
#define MSECOND 1000000
unsigned int cpu_nums;

struct apple
{
	unsigned long long a;
	char ch[128];
	unsigned long long b;
};
	
struct orange
{
	int a[ORANGE_MAX_VALUE];
	int b[ORANGE_MAX_VALUE];		
};
		
int set_cpu(int i)
{
	cpu_set_t mask;
	CPU_ZERO(&mask);
	CPU_SET(i,&mask);
	if(-1 == sched_setaffinity(gettid(),sizeof(&mask),&mask))return -1;
	return 0;
}

	
void* add(void* x)
{
	if(-1 == set_cpu(2))
	{
		return NULL;
	} 
		
	for(long long sum=0;sum<APPLE_MAX_VALUE;sum++)
	{
		((struct apple *)x)->a += sum;
		((struct apple *)x)->b += sum;
	}	
	
	return NULL;
}

double e = 0.2;
double T[11];
double avg;
int update(double time)
{
	if (time > (1+e)*avg || time < (1-e) * avg) return 1;
	for (int i = 1; i<= 10; ++i)
		if (time < T[i])
		{
			for (int j = 10; j >= i + 1; --j)
				T[j] = T[j - 1];
			T[i] = time;
			break;
		}

	//calculte avg
	int cnt = 0;
	double sum = 0;
	for (int i = 1; i <= 10; ++i)
	{
		if (T[i] < 900)
		{
			cnt ++;
			sum += T[i];
		}
	}

	avg = sum / cnt;
	return 0;
}

void* CalOrange(void * arg)
{
	struct orange* Orange = (struct orange *) arg;
	long long sum ;
	sum=0;
	for(long long i=0;i<ORANGE_MAX_VALUE; i++)
	{
		sum += Orange->a[i]+Orange->b[i];
	}
}

void Run()
{
	struct apple Apple;
	struct orange Orange;
	pthread_t ThreadA;
	
	cpu_nums = sysconf(_SC_NPROCESSORS_CONF);
	
	if(-1 == set_cpu(0))
	{
		return;
	} 
	
	pthread_create(&ThreadA,NULL,add,&Apple);
	
	CalOrange(&Orange);
		
	pthread_join(ThreadA,NULL);
}
	
int main (int argc, const char * argv[]) {

	for (int i = 1; i <= 10; ++i) T[i] = 999;
	
	for (int i = 1; i <= 200; ++i)
	{
		
		struct timeval start;
    	struct timeval end;
		gettimeofday(&start,NULL);
		Run();	
		gettimeofday(&end,NULL);
		long long endu = end.tv_usec;
		long long startu = start.tv_usec;
		double RunTime =(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000000.0;
		printf("%dth run, time = %.4lfs\n", i, RunTime);
		if (i == 1) avg = RunTime;
		if(update(RunTime))
		{
			printf("Non convergence!\n");
			return 0;
		}	
	}

	for (int i = 1; i <= 10; ++i)
	    printf("V[%d] = %.4lfs\n",i,T[i] );
	printf("Avg Time = %.4lfs\n", avg);		
	return 0;
}
