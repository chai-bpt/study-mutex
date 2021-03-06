#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <semaphore.h>

#define LOOP_COUNT_MAX 10000000

typedef struct __lock_t {
        pthread_mutex_t mutex;
}lock_t;

unsigned int uigGlobVar = 0;

lock_t lock_uigGlobVar;

void lock_init(lock_t* plock)
{
	pthread_mutex_init(&plock->mutex, NULL);
}

void lock_get(lock_t* plock)
{
	pthread_mutex_lock(&plock->mutex);
}

void lock_put(lock_t* plock)
{
	pthread_mutex_unlock(&plock->mutex);
}




void* ThreadFunc(void* vpTemp)
{	
	unsigned long ulLoopCount = 0;

	printf("\n\tThreadFunc_START\n");

	for(ulLoopCount = 0; ulLoopCount < LOOP_COUNT_MAX; ulLoopCount++)
	{
		lock_get(&lock_uigGlobVar);
		uigGlobVar++;
		lock_put(&lock_uigGlobVar);
	}

	printf("\n\tThreadFunc_END\n");
}




int main(int argvc, char* argv[])
{
	pthread_t thread1,thread2;

	printf("\n");
	printf("\n\tmain_START\n");

	printf("\n\tuigGlobVar = %d\n",uigGlobVar);

	lock_init(&lock_uigGlobVar);

	pthread_create( &thread1, NULL, ThreadFunc, NULL);
	printf("\n\tThread1 created\n");
      
       	pthread_create( &thread2, NULL, ThreadFunc, NULL);
        printf("\n\tThread2 created\n");

	pthread_join(thread1, NULL);
	printf("\n\tThread1 joined\n");
        
	pthread_join(thread2, NULL);
        printf("\n\tThread2 joined\n");

	printf("\n\tuigGlobVar = %d\n",uigGlobVar);

	printf("\n\tmain_END\n");
	printf("\n");
}


