#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
/*
Description Problem:
	There is a gas station with a gas tank to fill up cars.
	However, only one car can be filled up at a time.
	
Solution:
	
	Usamos o wait para verificar se o resultado da condição mudou.
	
	Com o signal, apenas um carro verificaria se há combustível suficiente
	para ele.
	
	Com broadcast, todas as threads (Carros) podem verificar se há combustível
	para cada uma.
	
Insights: Condition Variable
	
	-> wait:
		
		"
		This function blocks until the condition is signaled.
		
		In typical use, a condition expression is evaluated 
		under the protection of a mutex lock. When the condition
		expression is false, the thread blocks on the condition 
		variable.
		The condition that caused the wait must be retested before 
		continuing execution from the point of the pthread_cond_wait().
		
		Example:
		
			pthread_mutex_lock();
			while(condition_is_false){
				pthread_cond_wait();  // Vai esperar pelo sinal.
			}
			pthread_mutex_unlock();	
		
		"
		
	-> signal: 
	
		"
		Unblock one thread that is blocked on the condition variable by the wait.
		The scheduling policy determines the order in which blocked threads
		are awakened.
		
		"
		
		As threads estão em fila esperando ser 'acordadas' pelo signal, o qual
		somente acordará o primeiro.
		
	-> broad_cast:
		
		"
		Unblock all threads that are blocked on the condition variable specified 
		by pthread_cond_wait().
		Since pthread_cond_broadcast() causes all threads blocked on the condition
		to contend again for the mutex lock, use pthread_cond_broadcast() with care.
	
		"
*/

pthread_mutex_t bloq;
pthread_cond_t condic_fuel;
int fuel_on_time = 0;

int RATE_GIVEN_FOR_BOOKING = 50;
int RATE_TAKEN_BY_THE_CAR = 10;

void*
filling_up_from_reserve(){
	/*
	Description:
		Load the gas pump.
	*/
	for(
		int i = 0;
		i < 5;
		i++
	){
		pthread_mutex_lock(
			&bloq
		);
		
		fuel_on_time = fuel_on_time + RATE_GIVEN_FOR_BOOKING;
		printf(
			"Carregando bomba, ha %d.\n",
			fuel_on_time
		);
		
		pthread_mutex_unlock(
			&bloq
		);
		pthread_cond_broadcast(
			&condic_fuel
		);
		sleep(1);
	}
}

void*
unloading_into_a_car(
	void *arg
){
	/*
	Description:
		Load the car of the time.
	*/
	int id_car = *(int*)arg;
	
	pthread_mutex_lock(
		&bloq
	);
	while(
		fuel_on_time < RATE_TAKEN_BY_THE_CAR
	){
		printf(
			"Acabei de verificar, nao ha combustivel suficiente para carro %d.\n",
			id_car
		);
		
		pthread_cond_wait(
			&condic_fuel,
			&bloq
		);
	}
	
	fuel_on_time = fuel_on_time - RATE_TAKEN_BY_THE_CAR;
	printf(
		"Carro %d completo. Resta: %d.\n",
		id_car,
		fuel_on_time
	);
	pthread_mutex_unlock(
		&bloq
	);
	
	free(arg);
}


int main(){
	
	pthread_mutex_init(
		&bloq,
		NULL
	);
	pthread_cond_init(
		&condic_fuel,
		NULL
	);
	
	int number_of_cars = 4;
	pthread_t threads[number_of_cars + 1];
	
	for(
		int i = 0;
		i < (number_of_cars + 1);
		i++
	){
		if(
			i == (number_of_cars)
		){
			// The pump
			pthread_create(
				threads + i,
				NULL,
				filling_up_from_reserve,
				NULL
			);
		}
		else{
			
			int *id_car = malloc(sizeof(int));
			*id_car = i;
			
			pthread_create(
				threads + i,
				NULL,
				unloading_into_a_car,
				id_car
			);
		}
	}
	
	for(
		int i = 0;
		i < (number_of_cars + 1);
		i++
	){
		pthread_join(
			threads[i],
			NULL
		);
	}
	
	
	pthread_mutex_destroy(
		&bloq
	);
	pthread_cond_destroy(
		&condic_fuel
	);
	
	return 0;
}

