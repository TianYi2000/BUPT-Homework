//thread_create.c
/* thread_ create.c */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

/*线程函数1*/
void* mythread1(void)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		printf("I am the 1st pthread, Now i = %d\n", i);
		sleep(2);
	}
}
/*线程函数2*/
void* mythread2(void)
{
	int i;
	for (i = 0; i < 5; i++)
	{
		printf("I am the 2st pthread, Now i = %d\n", i);
		sleep(2);
	}
}

int main()
{
	pthread_t id1, id2;
	/*线程ID*/
	int res;
	/*创建一个线程，并使得该线程执行mythread1函数*/
	res = pthread_create(&id1, NULL,(void*)mythread1, NULL);
	if (res)
	{
		printf("Create pthread error!\n");
		return 1;
	}
	/*创建一个线程，并使得该线程执行mythread2函数*/
	res = pthread_create(&id2, NULL, (void*)mythread2, NULL);
	if (res)
	{
		printf("Create pthread error!\n");
		return 1;
	}
	/*等待两个线程均退出后，main()函数再退出*/
	pthread_join(id1, NULL);
	pthread_join(id2, NULL);
	return 1;
}

/*gcc thread_create.c -o thread_create -lpthread*/