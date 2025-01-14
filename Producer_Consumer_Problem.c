#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

/*
Also named by Bounded Buffer Problem:
	Two or more threads share the same buffer, ou stack.
	The Producer insere information in the buffer.
	The Consumer remove information.
	
Solution:
	-> Try changing the number of producers and consumers to get the better result.
*/

#define tam_buffer 20
sem_t sem_empty;
sem_t sem_full;
pthread_mutex_t bloq_buffer;
int buffer[tam_buffer] = {0};
int count_buffer = 0;

#define number_of_agents 10
#define delay 0.5

void*
producer(
	void* args
){
	int id = *(int*) args;
	
	srand(
		time(NULL) + id
	);
	
	while(
		1
	){
		int result = rand() % 100;
		sleep(delay);
		
		sem_wait(
			// Wait until there's one slot is empty
			&sem_empty
		);
		
		pthread_mutex_lock(
			&bloq_buffer
		);
		
		buffer[
			count_buffer
		] = result;
		
		printf(
			"Introduces %d in %d.\n",
			result,
			count_buffer
		);
		
		count_buffer++;
		
		pthread_mutex_unlock(
			&bloq_buffer
		);
		
		sem_post(
			// Notices that one slot is now ocuppied.
			&sem_full
		);
	}
	
	free(args);
}

void*
consumer(
	void* args
){
	
	while(
		1
	){
		sem_wait(
			
			&sem_full
		);
		
		pthread_mutex_lock(
			&bloq_buffer
		);
		
		int result = buffer[
			count_buffer - 1
		];
		
		printf(
			"Got %d in %d.\n",
			result,
			count_buffer - 1
		);
		
		count_buffer--;
		
		pthread_mutex_unlock(
			&bloq_buffer
		);
		
		sem_post(
			&sem_empty
		);
		
		sleep(delay);
	}
	free(args);
}

int main(){
	
	pthread_t agents[
		number_of_agents
	];
	
	void *(
		*agents_function[]
	)(void *) = {
		producer,
		consumer
	};
	
	pthread_mutex_init(
		&bloq_buffer,
		NULL
	);
	
	sem_init(
		&sem_empty,
		0,
		// Attention
		number_of_agents
	);
	
	sem_init(
		&sem_full,
		0,
		// Attention
		0 
	);
	
	
	for(
		int i = 0;
		i < number_of_agents;
		i++
	){
		int *id = (int*) malloc(sizeof(int));
		*id = i;
		
		pthread_create(
			agents + i,
			NULL,
			agents_function[
				i % 2
			],
			id
		);
	}
	
	for(
		int i = 0;
		i < number_of_agents;
		i++
	){
		pthread_join(
			agents[i],
			NULL
		);
	}
	
	pthread_mutex_destroy(
		&bloq_buffer
	);
	
	sem_destroy(
		&sem_empty
	);
	sem_destroy(
		&sem_full
	);
	
	return 0;
}
