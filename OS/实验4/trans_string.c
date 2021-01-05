#include <stdio.h>
#include <pthread.h>

void *create(char *arg)
{
	char *str;
	str = arg;
	printf("The parameter passed from main is %s\n",str);

	return (void *)0;
}

int main()
{
	int error;
	pthread_t id1;
	char *str1 = "Hello!";
	char *attr = str1;
	error = pthread_create(&id1, NULL, create, (void *)attr);

	if(error != 0)
	{
		printf("This pthread is not created!\n");
		return -1;
	}
	sleep(1);

	printf("pthread is created..\n");
	return 0;
}