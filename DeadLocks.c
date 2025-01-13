#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



pthread_mutex_t mutex_1;
pthread_mutex_t mutex_2;



void*
rotina(){
	if(
		rand() % 2
	){
		pthread_mutex_lock(
			&mutex_1
		);
		
		pthread_mutex_lock(
			&mutex_2
		);
	}
	else{
		pthread_mutex_lock(
			&mutex_2
		);
		
		pthread_mutex_lock(
			&mutex_1
		);
	}
	
	/*
	Exemplo do Erro:
		Note que h� a chance de haverem threads diferentes executando
		ordens diferentes de lock e unlock dentro dos m�ltiplos mutex's.
		
		Isso representa um perigo devido � rela��o de abertura e fechamento.
		Pois as threads devem estar em sincronia quanto aos mutex's, e 
		abrir, ou fechar, duas vezes o mesmo mutex representa um erro.
	*/
	
	
	pthread_mutex_unlock(
		&mutex_1
	);
	
	pthread_mutex_unlock(
		&mutex_2
	);
	
	
}

int main(){
	
	// ...	
	return 0;
}
