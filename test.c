#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define LOOP_COUNT_MAX 10000000
unsigned long ulgGlobVar = 0;

void* ThreadFunc(void* vpTemp)
{	
	unsigned long ulLoopCount = 0;

	printf("\n\tThreadFunc_START\n");

	for(ulLoopCount = 0; ulLoopCount < LOOP_COUNT_MAX; ulLoopCount++)
	{
		ulgGlobVar++;
	}

	printf("\n\tThreadFunc_END\n");
}

int main(int argvc, char* argv[])
{
	pthread_t thread1,thread2;

	printf("\n");
	printf("\n\tmain_START\n");

	printf("\n\tulgGlobVar = %lu\n",ulgGlobVar);

	pthread_create( &thread1, NULL, ThreadFunc, NULL);
	printf("\n\tThread1 created\n");
      
       	sleep(1);

       	pthread_create( &thread2, NULL, ThreadFunc, NULL);
        printf("\n\tThread2 created\n");

	pthread_join(thread1, NULL);
	printf("\n\tThread1 joined\n");
        
	pthread_join(thread2, NULL);
        printf("\n\tThread2 joined\n");

	printf("\n\tulgGlobVar = %lu\n",ulgGlobVar);

	printf("\n\tmain_END\n");
	printf("\n");
}


