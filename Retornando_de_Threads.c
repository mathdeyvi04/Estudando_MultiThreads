#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


void* 
jogar_dado(){
	/*
	
	*/
	int *resultado = malloc(sizeof(int)); 
	*resultado = rand();
	
	/*
	Aparentemente, tem que ser alocado mesmo, chora.
	
	resultado -> tipo int*
	
	(void*) resultado -> tipo void*
	
	*/
	return (void*) resultado;
}

int main(){
	
	srand(time(NULL));
	
	pthread_t th;
	
	if(
		pthread_create(
			&th,
			NULL,
			jogar_dado,
			NULL
		) != 0
	){
		printf("ERRO");
		return 1;
	}
	
	int *resultado = NULL;
	pthread_join(
		th, 
		/*
		
		&resultado -> possui tipo int **
		
		(void**) &resultado -> tipo void **
		
		*/
		(void**) &resultado
	);
	
	printf("Resultado: %d", *resultado);
	free(resultado);
	
	return 0;
}

