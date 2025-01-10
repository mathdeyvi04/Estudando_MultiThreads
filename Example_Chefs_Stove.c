#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/*
Description Problem:
	Chefs (threads) will try to access stove burners (variables).
	
Problems:
	-> Não verifica forno 0, diz que já está bloqueado, mas isso não
	   faz sentido.
	   
	-> Não usamos wait... :(
	   
*/

// Necessariamente menor ou igual à quantidade de chefs.
#define number_of_stoves_burners 3
pthread_mutex_t stove_mutex[
	number_of_stoves_burners
];

#define AMOUNT_INICIAL_FUEL 100
#define MAX_FUEL_NEEDED 30
int fuel_stove[
	number_of_stoves_burners
] = {0};

#define number_of_chefs 5

void*
acess_stove_burner(
	void *arg
){
	/*
	Tries to acess stove burner.
	
	Case 1: Stove Free
		Load and Lock.
	
	Case 2: Stove Lock
		Passes for next stove.
	
	Case 3: None Stove Free
		Wait until one is free.
	*/
	
	int fuel_needed_by_this_chef = (rand() % MAX_FUEL_NEEDED) + 1;
	int id_chef = *(int*)arg;
	
	printf(
		"Chef %d need %d of fuel for complete his task.\n\n",
		id_chef,
		fuel_needed_by_this_chef
	);
	
	int is_finished = 0;
	
	while(
		!is_finished
	){
		for(
			// Search for a stove burner available.
			int i = 0;
			i < number_of_stoves_burners;
			i++
		){

			if(
				!pthread_mutex_trylock(
					stove_mutex + i
				)
			){
				// Sucess on got the stove burner free.
				
				if(
					fuel_stove[i] >= fuel_needed_by_this_chef
				){
					printf(
						"Chef %d got the stove %d.\n\n",
						id_chef,
						i	
					);
					
					fuel_stove[i] = fuel_stove[i] - fuel_needed_by_this_chef;
					sleep(3);
					
					printf(
						"Stove %d is now free and Chef %d exited.\n\n",
						i,
						id_chef
					);
					
					pthread_mutex_unlock(
						stove_mutex + i
					);
					
					free(arg);
					
					is_finished = 1;
					
					break;
				}
				else{
					// There's no fuel suficient in this stove.
					// Search for another.
					pthread_mutex_unlock(
						stove_mutex + i
					);
				}
			}
		}
		
		// There's no stove burner free.
		printf(
			"Chef %d is waiting.\n\n",
			id_chef
		);
		sleep(1);
	}
}


int main(){
	
	pthread_t chefs[
		number_of_chefs
	];
	
	for(
		int i = 0;
		i < number_of_chefs;
		i++
	){
		if(
			i < number_of_stoves_burners
		){
			pthread_mutex_init(
				stove_mutex + i,
				NULL
			);
			
			fuel_stove[
				i
			] = AMOUNT_INICIAL_FUEL;
		}
		
		int *id_chef = (int*) malloc(sizeof(int));
		*id_chef = i;
		
		pthread_create(
			chefs + i,
			NULL,
			acess_stove_burner,
			id_chef
		);
	}
	
	
	for(
		int i = 0;
		i < number_of_chefs;
		i++
	){
		if(
			i < number_of_stoves_burners
		){
			pthread_mutex_destroy(
				stove_mutex + i
			);
		}
		
		pthread_join(
			chefs[i],
			NULL
		);
	}
	
	return 0;
}


