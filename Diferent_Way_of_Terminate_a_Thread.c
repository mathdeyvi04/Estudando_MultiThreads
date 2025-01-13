#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



void*
rotina(){
	printf(
		"Vendo dentro da thread."
	);
}


int main(){
	
	
	pthread_t th;
	
	pthread_create(
		&th,
		NULL,
		rotina,
		NULL
	);
	
	pthread_exit(0);
	
	/*
	Podemos usar o 'return' e o 'exit' para encerrar a main e TODOS os
	processos são cancelados.
	
	Da mesma forma, podemos executar 'pthread_exit()' para terminar e
	retornar algo de uma thread especifica.
	
		Caso usado na main, vai apenas terminar a thread principal e esperar
		pelas demais.
	
		Caso usado em outras, vai terminar elas e enviar como retorno.
	*/
	
	
	
	return 0;
}
