#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/*
Description Problem:
	Every thread rolls a dice, saved is value in an array.
	The main thread calculates the winner.
	
One Big Issue I Have:
	
	Part of the code:
			...
			while(
				number_of_match != max_of_match
			){
				
				int result = rand() % (2 * number_of_threads);
				
				pthread_mutex_lock(
					&locker_bigger_value
				);
				printf(
					"Sou thread %d, meu valor %d.\n",
					id_player,
					result
				);
				pthread_mutex_unlock(
					&locker_bigger_value
				);
				
				pthread_barrier_wait(
					&barrier_match
				);
				
				
			}
			...

			...
			// Execution of the Main Thread.
			while(
				number_of_match != max_of_match
			){
				printf(
					"\tMatch %d:\n\n",
					number_of_match + 1
				);
				
				// Wait until all plays.
				pthread_barrier_wait(
					&barrier_match
				);		
				
				// End of the match.
				number_of_match++;
			}
			...
	
	Note como após a primeira barreira, todas as threads são liberadas.
	
	Algumas threads são executadas antes da main conseguir avançar a 
	partida.		
	
	Ao atualizar para o valor limite, as outras threads que ainda não
	haviam sido executadas, terminam devido ao fim do loop, o qual
	também se encerra na main.
	
	Entretanto, devido ao join da main e ao wait das threads que foram
	executadas, o programa nunca encerra.
*/

#define number_of_threads 10
#define max_of_match 3

pthread_mutex_t locker_bigger_value;
pthread_barrier_t barrier_match;
int winner[2] = {-1, 0};

int number_of_match = 0;


void*
roll_dice(
	void *arg
){
	int id_player = *(int*) arg;
	srand(
		// Solution for the problem of random numbers not random.
		time(NULL) + id_player
	);
	
	while(
		number_of_match != max_of_match
	){
		
		int result = rand() % (2 * number_of_threads);
		
		// Wait until all players are in condition
		pthread_barrier_wait(
			&barrier_match
		);
		
		// Forces the linear projection
		pthread_mutex_lock(
			&locker_bigger_value
		);
		printf(
			"Sou jogador %d, obtive %d.\n",
			id_player,
			result,
			number_of_match
		);
		
		if(
			result > winner[0]
		){
			winner[0] = result;
			winner[1] = id_player;
		}
		
		pthread_mutex_unlock(
			&locker_bigger_value
		);
		
		// Wait until all plays.
		pthread_barrier_wait(
			&barrier_match
		);
		
		// Wait until the main flips the match.
		pthread_barrier_wait(
			&barrier_match
		);
		
	}
	
	free(arg);
}

int main(){
	
	pthread_t players[number_of_threads];
	pthread_mutex_init(
		&locker_bigger_value,
		NULL
	);
	
	pthread_barrier_init(
		&barrier_match,
		NULL,
		// Mais 1 para a main
		number_of_threads + 1 
	);
	
	for(
		int i = 0;
		i < number_of_threads;
		i++
	){
		int *arg = (int*) malloc(sizeof(int));
		*arg = i + 1;
		
		pthread_create(
			players + i,
			NULL,
			roll_dice,
			arg
		);
	}
	
	// Execution of the Main Thread.
	while(
		number_of_match != max_of_match
	){
		printf(
			"\tMatch %d:\n\n",
			number_of_match + 1
		);
		
		// Wait until all players are in condition
		pthread_barrier_wait(
			&barrier_match
		);
		
		// Wait until all plays.
		pthread_barrier_wait(
			&barrier_match
		);		
		
		printf(
			"\nThe winner is the player %d.\n",
			winner[1]
		);
		
		winner[0] = -1;
		
		// End of the match.
		number_of_match++;
		
		// Wait until the main flips the match.
		pthread_barrier_wait(
			&barrier_match
		);	
	}
	
	
	
	
	for(
		int i = 0;
		i < number_of_threads;
		i++
	){
		pthread_join(
			players[i],
			NULL
		);
	}
	
	
	pthread_mutex_destroy(
		&locker_bigger_value
	);
	
	pthread_barrier_destroy(
		&barrier_match
	);
	
	return 0;
}
