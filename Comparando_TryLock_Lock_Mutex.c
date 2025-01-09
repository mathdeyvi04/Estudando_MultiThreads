#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t bloq;

void*
rotina(){
	
	printf("Tentando acessar.\n");
	pthread_mutex_lock(&bloq);
	/*
	Usando lock:
		Todas começam a executar, entretanto, o lock no mutex força que
		apenas uma thread tenha acesso à execução do bloco enquanto todas
		as outras estão PARADAS NO TEMPO.
		
		"
			When pthread_mutex_lock() returns, the mutex is locked. The calling thread is the owner. If
			the mutex is already locked and owned by another thread, the calling thread blocks until the
			mutex becomes available.
		"
		
	Usando trylock:
		Tentará bloquear o mutex.
		Caso consiga, retorna 0.
		Caso não, outra coisa.
		
		Com isso, conseguimos continuar o fluxo de cada uma das threads.
		
		if (trylock() == 0){
			//
			unlock();
		else{
			// Já estava lock, vamos continuar.	e faz outras coisas.
		}
	*/
	
	printf("Hop\n");
	
	sleep(1);
	
	pthread_mutex_unlock(&bloq);
	
}

int main(){
	
	pthread_t threads[4];
	pthread_mutex_init(
		&bloq,
		NULL
	);
	
	for(
		int i = 0;
		i < 4;
		i++
	){
		if(
			pthread_create(
				threads + i,
				NULL,
				rotina,
				NULL
			) != 0
		){
			printf("Deu ruim.");
		}
	}
	
	for(
		int i = 0;
		i < 4;
		i++
	){
		pthread_join(
			threads[i],
			NULL
		);
	}
	
	pthread_mutex_destroy(&bloq);
	
	return 0;
}


