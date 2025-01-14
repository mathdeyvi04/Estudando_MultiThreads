#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define number_of_threads 4

sem_t semap;

/*
Descrição:
	
	Age de forma semalhante ao mutex, entretanto, é mais poderoso.
	
	sem_wait:
		
		semap == 0:
			
			Lock and wait until semap > 0.
			
		semap > 0:
			
			semap--;
			
			Continue the execution.
	
	sem_post:
		
		semap++;

*/

void*
rotina(
	void *args
){
	/*
	
	O fluxo do programa:
	
	T0 - . D P I
	T1 - . . . . D P I
	T2 - . . . . . . . D P I
	T3 - . . . . . . . . . . D P I
   SEM - 1 0 0 1 0 0 1 0 0 1 0 0 1
	
	D -> Decrement by the sem_wait.
	I -> Increment by the sem_post.
	P -> printf
	
	Observe que se o valor de semap for maior no inicio,
	mais threads poderão ser executadas ao mesmo tempo enquanto
	demais apenas esperarão.
	
	T0 - . D P I
	T1 - . D P I
	T2 - . . . . D P I
	T3 - . . . . D P I 
   SEM - 2 0 0 2 0 0 2
   
	*/
	
	
	int id = *(int*)args;
	
	sem_wait(
		&semap
	);
	
	sleep(1);
	
	printf("Sou thread %d.\n", id);
	
	sem_post(
		&semap
	);
	
	free(args);
}

int main(){
	
	pthread_t threads[number_of_threads];
	
	sem_init(
		&semap,
		0,
		2
	);
	
	for(
		int i = 0;
		i < number_of_threads;
		i++
	){
		int *arg = (int*) malloc(sizeof(int));
		*arg = i;
		pthread_create(
			threads + i,
			NULL,
			rotina,
			arg
		);
	}
	
	for(
		int i = 0;
		i < number_of_threads;
		i++
	){
		pthread_join(
			threads[i],
			NULL
		);
	}
	
	sem_destroy(
		&semap
	);
}

