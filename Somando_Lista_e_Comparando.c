#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>


int*
get_list_of_values(
	int number_of_values
){
	/*
	Criará lista do tamanho que desejamos.
	*/
	
	int *list_of_values = malloc(
		number_of_values * sizeof(int)
	);
	
	for(
		int i = 0;
		i < number_of_values;
		i++
	){
		list_of_values[i] = (rand() % number_of_values) + 1;
	}
	
	return list_of_values;
}

int
unsynchronized_function(
	int *values,
	int number_of_values	
){
	/*
	Without using thread, it will add the numbers in the list.
	*/
	
	int sum = 0;
	for(
		int i = 0;
		i < number_of_values;
		i++
	){
		sum = sum + values[i];
	}
	
	return sum;
}

int
synchronized_function(
	int *list_of_values,
	int quantity_of_numbers,
	int number_of_threads
){
	/*
	Using Thread, sum the list.
	*/
	
	void*
	soma_responsavel_por_cada_thread(
		void *argumento
	){
		/*
		Function to be performed within the thread.
		
		Algorithm Apresentation:
		
		Denote:
			quantity_of_numbers = Q
			number_of_thread = n
		
		Know that Q % n = 0.
		
		Take as example: Q = 6 and n = 2.
		
		id_th = 0:
			Access 0, 1, 2.
		
		id_th = 1: 
			Acess 3, 4, 5.
			
		So,
		
		id_th = i:
			Acess i * (Q // n), ..., (i + 1) * (Q // n) - 1.		
		*/
		
		int id_th = *(int*)argumento;
		int sum = 0;
		for(
			int i = (
				quantity_of_numbers / number_of_threads
			) * id_th; 
			i < (
				quantity_of_numbers / number_of_threads
			) * (id_th + 1);
			i++
		){
			sum += list_of_values[i];
		}
				
		*(int*)argumento = sum;
		
		return argumento;
	}
	
	pthread_t threads[number_of_threads];
	for(
		int i = 0;
		i < number_of_threads;
		i++
	){
		int *ponteiro_de_id = malloc(sizeof(int));
		*ponteiro_de_id = i;
		
		if(
			pthread_create(
				threads + i,
				NULL,
				soma_responsavel_por_cada_thread,
				ponteiro_de_id
			) != 0
		){
			printf("Deu ruim ao criar thread %d\n", i);
		}
	}
	
	int global_sum = 0;
	for(
		int i = 0;
		i < number_of_threads;
		i++
	){
		int *res = 0;
		
		if(
			pthread_join(
				threads[i],
				(void**) &res
			) != 0
		){
			printf("Deu erro ao finalizar thread %d\n", i);
		}
		
		global_sum = global_sum + *res;
		free(res);  // liberando a memória alocada para cada argumento.
	}

	return global_sum;	
}	
	
int main(){
	
	int quantity = 1000;
	int *list_of_values = get_list_of_values(
		quantity  // Tamanho da lista de valores aleatorios entre 1 e quantity.
	);
	
	
	int resultado = unsynchronized_function(
		list_of_values,
		quantity
	);
	printf("Soma: %d", resultado);
	
	int NUMBER_OF_THREADS = 10;
	int resultado = synchronized_function(
		list_of_values,
		quantity,
		NUMBER_OF_THREADS
	);
	printf("\nSoma: %d", resultado);
	
	free(list_of_values);
	return 0;
}






