#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int PRIMOS[10] = {
	/*
	SITUA��O PROBLEMA.
	Em geral, temos uma thread lendo um array de 10 elementos.
	Aqui, desejamos ter n threads lendo 10 / n elementos do array.
	*/
	2, 3, 5, 7, 11,
	13, 17, 19, 23, 29
};

void*
funcao_a_acessar_array(
	void *argumento  // Devemos passar um ponteiro como argumento.
){
	printf(
		"Sou a thread %d e posso come�ar a ler elementos.\n",
		*(int*)argumento + 1
	);
	
	free(argumento);
}

int main(){
	
	int NUMBER_THREADS = 2; 
	pthread_t threads[NUMBER_THREADS];
	
	for(
		int i = 0;
		i < NUMBER_THREADS;
		i++
	){
		// Ver coment�rio grande dps.
		int *valor_a_ser_passado = malloc(sizeof(int));
		*valor_a_ser_passado = i;
		
		if (
			pthread_create(
				threads + i,
				NULL,
				funcao_a_acessar_array,
				/*
				-> Apresenta��o do Problema:
				
					Desejamos acessar elementos espec�ficos do array,
					para isso vamos passar este i local como argumento: &i.
					Note que estaremos passando um ponteiro, um local na 
					mem�ria.
					
					Entre o tempo de criar uma thread e esta come�ar a exe-
					cutar, o valor que este ponteiro aponta se altera e, com
					isso, perdemos o argumento verdadeiro que foi passado.
					
				-> Solu��o:
					
					-> Aloca��o de Mem�ria Diferente:
						Garantindo endere�os de mem�ria diferentes para cada
						threads, resolvemos o problema aumento o espa�o utilizado.
					
					-> Usando mutex:
						Impedimos que o valor que est� neste ponteiro seja alterado.
				*/
				valor_a_ser_passado				
			) != 0
		){
			perror("Falha ao criar thread");
		}
	}
	
	for(
		int i = 0;
		i < NUMBER_THREADS;
		i++
	){
		if (
			pthread_join(
				threads[i],
				NULL
			) != 0
		){
			perror("Falha ao finalizar thread");
		}
	}
	
	return 0;
}






