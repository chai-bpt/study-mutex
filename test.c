#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <string.h>
#include <semaphore.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <stdlib.h>

#define LOOP_COUNT_MAX 10000000
/*
typedef struct __lock_t {
	sem_t sem ;
}lock_t;
*/

#define lock_t u_int32_t
unsigned int uigGlobVar = 0;

lock_t lock_uigGlobVar;

static int futex( u_int32_t *uaddr, int futex_op, u_int32_t val, const struct timespec *timeout, u_int32_t *uaddr2, u_int32_t val3)
{
           return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

static int futexOps(u_int32_t *uaddr, int futex_op, u_int32_t val)
{
           return futex(uaddr, futex_op, val, NULL, NULL, 0);
}

static int futexOpsWait(u_int32_t *uaddr)
{
           return futexOps(uaddr, FUTEX_WAIT, 0);
}

static int futexOpsPost(u_int32_t *uaddr)
{
           return futexOps(uaddr, FUTEX_WAKE, 1);
}

static int futexOpsInit(u_int32_t *uaddr)
{
           *uaddr = 1;
}

void lock_init(lock_t* plock)
{
//	sem_init(&plock->sem, 0, 1);
	futexOpsInit(plock);
}

void lock_get(lock_t* plock)
{
//	sem_wait(&plock->sem);
	while(1)
	{
		if(__sync_val_compare_and_swap(plock, 1, 0) == 1)
		{
	        	break;
		}
		else
		{
	            	futexOpsWait(plock);
		}
	}
}


void lock_put(lock_t* plock)
{
//	sem_post(&plock->sem);
        __sync_val_compare_and_swap(plock, 0, 1);
        futexOpsPost(plock);
}




void* ThreadFunc(void* vpTemp)
{	
	unsigned long ulLoopCount = 0;

	printf("\n\tThreadFunc_START\n");

	for(ulLoopCount = 0; ulLoopCount < LOOP_COUNT_MAX; ulLoopCount++)
	{
		lock_get(&lock_uigGlobVar);
		//printf("\n\t[%ld]",pthread_self());
		uigGlobVar++;
		lock_put(&lock_uigGlobVar);
		//sleep(1);
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


