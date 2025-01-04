#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int PRIMOS[10] = {
	/*
	SITUAÇÃO PROBLEMA.
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
		"Sou a thread %d e posso começar a ler elementos.\n",
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
		// Ver comentário grande dps.
		int *valor_a_ser_passado = malloc(sizeof(int));
		*valor_a_ser_passado = i;
		
		if (
			pthread_create(
				threads + i,
				NULL,
				funcao_a_acessar_array,
				/*
				-> Apresentação do Problema:
				
					Desejamos acessar elementos específicos do array,
					para isso vamos passar este i local como argumento: &i.
					Note que estaremos passando um ponteiro, um local na 
					memória.
					
					Entre o tempo de criar uma thread e esta começar a exe-
					cutar, o valor que este ponteiro aponta se altera e, com
					isso, perdemos o argumento verdadeiro que foi passado.
					
				-> Solução:
					
					-> Alocação de Memória Diferente:
						Garantindo endereços de memória diferentes para cada
						threads, resolvemos o problema aumento o espaço utilizado.
					
					-> Usando mutex:
						Impedimos que o valor que está neste ponteiro seja alterado.
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






