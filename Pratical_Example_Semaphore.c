#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>


#define number_of_players 5
#define max_of_players_in_a_queue 3

sem_t semap_match;


void*
player_move(
	void *args
){
	int preference = *(int*) args;
	
	srand(
		time(NULL) + preference
	);
	
	printf(
		"ID: %d -- Wait for the login queue.\n",
		preference
	);
	
	sem_wait(
		&semap_match
	);
	
	printf(
		"ID: %d -- Login in.\n",
		preference
	);
	
	sleep(rand() % 5 + 1);
	
	printf(
		"ID: %d -- Login out.\n",
		preference
	);
	
	sem_post(
		&semap_match
	);
	
	free(args);
}

int main(){
	
	pthread_t players[number_of_players];
	
	sem_init(
		&semap_match,
		0,
		max_of_players_in_a_queue
	);
	
	for(
		int i = 0;
		i < number_of_players;
		i++
	){
		int *preference = (int*) malloc(sizeof(int));
		*preference = i;
		
		pthread_create(
			players + i,
			NULL,
			player_move,
			preference
		);
	}
	
	for(
		int i = 0;
		i < number_of_players;
		i++
	){
		pthread_join(
			players[i],
			NULL
		);
	}
	

	
	sem_destroy(
		&semap_match
	);

	return 0;
}
