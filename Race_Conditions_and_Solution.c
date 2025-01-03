#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int total = 0;
int valor_a_ser_somado = 10000;

pthread_mutex_t bloqueador;  // Criamos aqui para usar na função sem passar argumento.

void*
funcao_a_ser_assincrona(){
/*
Apresentação do Problema:
	
	-> Race Conditions:
		
		-> Para valores pequenos de 'valor_a_somado':
		
			A thread 1 executa tão rapidamente que antes que a thread 2
			seja iniciada, a 1 já encerrou seus trabalhos. Por isso, é 
			como se trabalhassem de forma não sincrona.
			
		
		-> Para valores grandes de 'valor_a_somado':
		
			As threads passam a executar simultaneamente. Entretanto, não
			necessariamente na mesma velocidade! E aqui nasce o problema,
			pois o OS não necessariamente dará a mesma atenção às duas.
			
			Suponha th_1 e th_2 e elas se desincronizam durante a execução de th_2.
			
			Ambas leem total como 10, por exemplo. th_1 pode executar algumas somas
			enquanto th_2 está parada.
			
			De repente, th_2 volta e o valor que estava guardado localmente é 10, não
			o valor atualizado por th_1. Daí, total volta a ser 11, ou seja, perde valor
			devido ao acesso não saudável da memória.
	
	-> Solução:
		
		-> Criar Bloqueadores:
			
			int variavel_esta_bloqueada = 0;
			
			if (
				!variavel_esta_bloqueada
			){
				variavel_esta_bloqueada = 1;
				
				// ACESSA VARIAVEL
				
				variavel_esta_bloqueada = 0;
			}
			
			Dessa forma, o local da memória está protegida.
			
		-> Usar o mutex:
*/

	for(
		int i = 0;
		i < valor_a_ser_somado;
		i++
	){
		pthread_mutex_lock(&bloqueador);
		total++;
		pthread_mutex_unlock(&bloqueador);
	}
}

int main(){
	int NUMBERS_THREADS = 5;
	pthread_t opcoes_threads[NUMBERS_THREADS];
	
	/*
	SOLUTION
	*/
	pthread_mutex_init(
		&bloqueador,
		NULL
	);
	
	for(
		int i = 0;
		i < NUMBERS_THREADS;
		i++
	){
		pthread_create(
			opcoes_threads + i,
			NULL,
			funcao_a_ser_assincrona,
			NULL
		);
	}
	
	for(
		int i = 0;
		i < NUMBERS_THREADS;
		i++
	){
		pthread_join(
			opcoes_threads[i],
			NULL
		);
	}
	
	/*
	SOLUTION
	*/
	pthread_mutex_destroy(
		&bloqueador
	);
	
	printf("Resultado: %d", total);

	return 0;
}












