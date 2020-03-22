/*
	Example program for condition variable based syncronization between two threads
	where both threads require to print a set of values in order.
*/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
pthread_mutex_t count_mutex	= PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var	= PTHREAD_COND_INITIALIZER;

void *func1();
void *func2();

int count = 0;

#define COUNT_DONE 	10
#define HALT_1		3
#define HALT_2		6

void main() {
	pthread_t t1, t2;

	printf("########################################## \n");
	printf("########################################## \n");
	printf("########################################## \n");
	pthread_create(&t1, NULL, &func1, NULL);
	pthread_create(&t2, NULL, &func2, NULL);


	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	printf("########################################## \n");
	printf("########################################## \n");
	printf("########################################## \n");
	printf("Final count is %d\n", count);

	exit(0);

}

void *func1() {
	for(;;) {

		/* Lock the mutex and wait for second thread's signal to unlock it */
		pthread_mutex_lock(&count_mutex);
		printf("Function 1 Acquired Lock. Waiting for cond var\n");
		/* pthread_cond_wait will release the count_mutex mutex and will wait till condition_var is set */
		pthread_cond_wait(&condition_var, &count_mutex);	
		count++;
		printf("Func 1 value: %d. And releasing lock \n", count);
		pthread_mutex_unlock(&count_mutex);
		
		if(count >= COUNT_DONE) {
			return;
		}
	}
}

void *func2() {
	for(;;) {

		//pthread_mutex_lock(&count_mutex);
		if(count < HALT_1 || count > HALT_2) {
			/* Function 1 is now permitted to modify count */
			printf("Function 2 Acquired lock & Sending Signal.\n");
			/* pthread_cond_signal before setting the condition_var requires that the lock is acquired first */
			pthread_cond_signal(&condition_var);
		}
		else {
			count++;
			printf("Func 2 Aqcuired lock and value: %d.\n", count);
		}
		
		printf("Func 2 Releasing lock\n");
		pthread_mutex_unlock(&count_mutex);
		
		if(count >= COUNT_DONE) {
			return;
		}
	}
}
