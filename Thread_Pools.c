#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

typedef struct {
	/*
	Task will be just the sum of two numbers.
	*/
	
	int a, b;
} Task;

#define number_of_threads 10
#define number_of_tasks 20

#define max_number_of_task 10
Task task_queue[
	max_number_of_task
];
int task_count = 0;

pthread_mutex_t bloq_queue;
pthread_cond_t cond_queue;

void 
execute_task(
	Task task_target
){
	printf(
		"The sum of %d and %d is: %d.\n",
		task_target.a,
		task_target.b,
		task_target.a + task_target.b
	);
	sleep(1);
}

void 
submit_task(
	Task task_target
){
	
	pthread_mutex_lock(
		&bloq_queue
	);
	
	task_queue[
		task_count
	] = task_target;
	
	task_count++;
	
	pthread_mutex_unlock(
		&bloq_queue
	);
	
	// With signal, the one task is execute by one thread.
	pthread_cond_signal(
		&cond_queue
	);
}

void*
start_thread(
	void *args
){
	int index = 0;
	
	while(
		1
	){
		
		Task task;
		
		// The acess to our task_queue can not be parallel.
		pthread_mutex_lock(
			&bloq_queue
		);
		
		while(
			!task_count
		){	
			// Wait until there's at the least one task.
			// The most important detail for the optmize the solution.
			pthread_cond_wait(
				&cond_queue,
				&bloq_queue
			);
		}
		
		
		// The first is choisen.
		task = task_queue[0];
		
		// The following are move to the left.
		index = 0;
		while(
			index < (task_count - 1)
		){
			
			task_queue[
				index
			] = task_queue[
				index + 1
			];
			
			index++;
		}
			
		task_count--;	
	
		pthread_mutex_unlock(
		&bloq_queue
		);
		
		// Execute the task out mutex
		// In parallel.
		execute_task(
			task
		);
		
	}
	
}

int main(){
	
	pthread_mutex_init(
		&bloq_queue,
		NULL
	);
	
	pthread_cond_init(
		&cond_queue,
		NULL
	);
	
	pthread_t threads[
		number_of_threads
	];
	
	for(
		int i = 0;
		i < number_of_threads;
		i++
	){
		pthread_create(
			threads + i,
			NULL,
			start_thread,
			NULL
		);
	}
	
	// Creating the tasks.
	for(
		int i = 0;
		i < number_of_tasks;
		i++
	){
		srand(
			time(NULL) + i 
		);
		
		Task task_target = {
			.a = rand() % 100,
			.b = rand() % 100
		};

		submit_task(
			task_target
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
	
	pthread_mutex_destroy(
		&bloq_queue
	);
	
	pthread_cond_destroy(
		&cond_queue
	);
	
	return 0;
}
