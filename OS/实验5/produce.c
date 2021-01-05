#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
int count_A=0, count_B=0;   	//分别表示当前工作台中已有的part A和part B的数量
int empty=12;            		// 工作台中空位数量, empty=12 - ( count_A+ count_B)
pthread_mutex_t mutexA, mutexB;	//二元信号量，分别用于控制对count_A、count_B互斥访问
pthread_mutex_t mutexStation;  		//二元信号量，控制对工作台和empty的互斥访问（取/放零件）
pthread_mutex_t suspend_A, suspend_B,	//控制台无足够空位时，挂起worker A、B
suspend_C;					//控制台无足够零件时，挂起worker C

void procA()
/*worker A生产part A：*/
{
	while (1) {
		pthread_mutex_lock (&mutexA);  //申请互斥访问count_A、empty，判断工作台留给part A的空位数
		pthread_mutex_lock (&mutexStation);    //申请对控制台和empty的访问权
		if  ((count_A <=7) && (empty >= 2))   {
			count_A = count_A + 2;    //修改count_A
			empty = empty - 2;        //修改empty
			printf ("worker A produces 2 partA, Now partA = %d, partB = %d\n", count_A, count_B);

			pthread_mutex_unlock(&mutexStation);         //释放对控制台和empty的访问权
			pthread_mutex_unlock(&mutexA);

			pthread_mutex_unlock(&suspend_C);      //控制台放入新零件，解挂C
		}  
		else  { 
			pthread_mutex_unlock(&mutexStation);    //释放对控制台和empty的访问权
			pthread_mutex_unlock(&mutexA);
			pthread_mutex_lock(&suspend_A);  //控制台无足够空位，或不允许再放入part A，转入waiting态，挂起自身
		} 
		sleep(1);
	}
}


void procB()
/*worker B生产part B：*/
{
	while (1) {	
		pthread_mutex_lock(&mutexB);  //申请互斥访问count_B、empty，判断工作台留给part B的空位数
		pthread_mutex_lock(&mutexStation);    //申请对控制台和empty的访问权
		if  ((count_B <=7) && (empty >= 1))   {
			count_B = count_B + 1;    //修改count_B
			empty = empty - 1;        //修改empty
			printf ("worker B produces 1 partB, Now partA = %d, partB = %d\n", count_A, count_B);

			pthread_mutex_unlock(&mutexStation);         //释放对控制台和empty的访问权
			pthread_mutex_unlock(&mutexB);

			pthread_mutex_unlock(&suspend_C);      //控制台放入新零件，解挂C
		}  
		else  { 
			pthread_mutex_unlock(&mutexStation);    //释放对控制台和empty的访问权
			pthread_mutex_unlock(&mutexB);
			pthread_mutex_lock(&suspend_B);  //控制台无足够空位，或不允许再放入part B，转入waiting态，挂起自身
		}
		sleep(1); 
	}
}

void procC()
/*worker C装配：*/
{
	while (1) {
		pthread_mutex_lock (&mutexA);              //申请互斥访问count_A，判断part A数量
		pthread_mutex_lock (&mutexB);              //申请互斥访问count_B，判断part B数量
		if  ((count_A >=4) && (count_B >=3)) { //有足够多的part A, part B用于装配
			pthread_mutex_lock(&mutexStation);        //申请对控制台的访问权

			/*同时取出4个part A、3个part B;*/
			count_A = count_A - 4;    //修改count_A
			count_B = count_B - 3;    //修改count_B
			printf ("worker C consume 4 partA and 3 partB, Now partA = %d, partB = %d\n", count_A, count_B);

			/*生产1个part C;*/
			empty = empty + 7; 
			pthread_mutex_unlock(&mutexStation);
					
			pthread_mutex_unlock(&mutexA);  
			pthread_mutex_unlock(&mutexB);

			pthread_mutex_unlock(&suspend_A);     //控制台新增空位，解挂A, B
			pthread_mutex_unlock(&suspend_B);
		}  
		else {
			pthread_mutex_unlock(&mutexA);  
			pthread_mutex_unlock(&mutexB);
			pthread_mutex_lock(&suspend_C);    //控制台无足够零件，转入waiting态，挂起自身
		}
		sleep(1);
	}
}



void thread_create()
{
	pthread_t thrdA,thrdB,thrdC;
	if(pthread_create(&thrdA,NULL,procA,NULL) != 0)
	{
		printf("create thread A failed\n");
		exit(1); 
	}
	if(pthread_create(&thrdB,NULL,procB,NULL) != 0)
	{
		printf("create thread B failed\n");
		exit(1); 
	}
		
	if(pthread_create(&thrdC,NULL,procC,NULL) != 0)
	{
		printf("create thread C failed\n");
		exit(1); 
	}
	
}

void mutex_create()
{
	pthread_mutex_init(&mutexA,NULL);
	pthread_mutex_init(&mutexB,NULL);
	pthread_mutex_init(&mutexStation,NULL);
	pthread_mutex_init(&suspend_A,NULL);
	pthread_mutex_init(&suspend_B,NULL);
	pthread_mutex_init(&suspend_C,NULL);

	pthread_mutex_lock(&suspend_A);
	pthread_mutex_lock(&suspend_B);
	pthread_mutex_lock(&suspend_C);
}

void mutex_destroy()
{
	pthread_mutex_destroy(&mutexA);
	pthread_mutex_destroy(&mutexB);
	pthread_mutex_destroy(&mutexStation);
	pthread_mutex_destroy(&suspend_A);
	pthread_mutex_destroy(&suspend_B);
	pthread_mutex_destroy(&suspend_C);
}

int main()
{
	printf ("System start\n");
	printf ("Begin to create mutex...\n");
	mutex_create();
	printf ("Begin to create threads...\n");
	thread_create();
	sleep(100);
	printf("System exiting and destroy mutex...\n");
	mutex_destroy();
}