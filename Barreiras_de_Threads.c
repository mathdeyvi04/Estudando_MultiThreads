#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>


/*

pthread_barrier_t

	Imagine uma quantidade X de threads sendo executadas.
	
	Caso a barreira necessite de uma quantidade Y de threads para permitir
	a passagem de todas, ela irá travar as demais threads que chegarem ATÉ
	que se tenha as Y threads.
	
	Casos Possíveis:
		
		X < Y:
			
			Barreira travará execução de todo o código.
			
		X >= Y:
		
			Sempre que chegar na quantidade Y, a barreira liberará as threads
			que chegaram até o momento, bloqueando novamente a passagem depois.
			O processo se repete.	

*/

#define number_of_threads 5
#define number_of_threads_which_may_pass 5

pthread_barrier_t barreira;

typedef struct {
	int inicio;
	int id;
} Arg;

void*
rotina(
	void *arg
){
	/*
	
	*/
	
	Arg exemplo = *(Arg*)arg;
	
	sleep(exemplo.id);
	
	printf(
		"INSTANTE: %d - Thread %d bateu na barreira.\n",
		time(NULL) - exemplo.inicio,
		exemplo.id
	);
	
	if(
		exemplo.id == 5
	){
		printf("\n");
	}
		
	pthread_barrier_wait(
		&barreira
	);

	printf(
		"INSTANTE: %d - Thread %d atravessou a barreira.\n",
		time(NULL) - exemplo.inicio,
		exemplo.id
	);
	
	free(arg);	
}

int main(){
	
	int inicio_do_tempo = time(NULL);
	
	pthread_t threads[number_of_threads];
	pthread_barrier_init(
		&barreira,
		NULL,
		number_of_threads_which_may_pass
	);
	
	printf("0 - Inicio.\n\n");
	
	for(
		int i = 0;
		i < number_of_threads;
		i++
	){
		
		Arg *a_ser_passado = (Arg*) malloc(sizeof(Arg));
		(*a_ser_passado).inicio = inicio_do_tempo;
		(*a_ser_passado).id = i + 1;
		
		pthread_create(
			threads + i,
			NULL,
			rotina,
			a_ser_passado
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
	
	pthread_barrier_destroy(
		&barreira
	);
	
	return 0;
}
