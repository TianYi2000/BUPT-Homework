#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

struct menber
{
	int a;
	char *s;
};

void *create(void *arg)
{
	struct menber *temp;
	temp = (struct menber *)arg;
	printf("menber->a = %d\n",temp->a);
	printf("menber->s = %s\n",temp->s);

	return (void *)0;
}

int main()
{
	int error;
	pthread_t id1;
	struct menber *p;
	p = (struct menber *)malloc(sizeof(struct menber));
	p->a = 1;
	p->s = "Robben!";

	error = pthread_create(&id1,NULL,create,(void *)p);

	if(error)
	{
		printf("pthread is not created!\n");
		return -1;
	}
	sleep(1);
	printf("pthread is created!\n");

	free(p);
	p = NULL;
	return 0;
}