#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void*
funcao_a_ser_assincrona(){
	/*
	Executada pela thread que a chamar. Atenção ao tipo.
	
	Caso não fosse assíncrono, a saída seria:
		
		Comecei uma thread.
		Encerrei uma thread.
		Comecei uma thread.
		Encerrei uma thread.
		
	Mas sabemos que não é isso.
	
	ELAS COMPARTILHAM A MESMA MEMÓRIA.
 	*/
	
	printf("Comecei uma thread.\n");
	
	sleep(1);
	
	printf("\nEncerrei uma thread.\n");
	
}

int main(){
	// Variável que armazenará a informação da thread.
	pthread_t exemplo_de_thread_1;
	pthread_t exemplo_2;
	
	
	if (
		pthread_create(
		
			// Endereço de memória da variável que armazenará a thread.
			&exemplo_de_thread_1,
			
			// Default
			NULL,
			
			
			/*
			-------------------------------------PARTE MAIS COMPLEXA--------------------------
			-> void *(*func)(void*)
			
			void *
				O tipo de retorno da função é genérico. Isso significa que pode ser convertido
				para qualquer tipo, desde que feito corretamente.
			
			(*func)
				Indica que 'func' é o ponteiro para a função, não a função diretamente.
				Isto significa que deve ser passado o endereço de memória da função.
			
			(void*)
				Indica que a função apontada por 'func' aceita um único argumento,
				que é um ponteiro genérico.
			*/
			funcao_a_ser_assincrona,
			
			// Por agora, não colocaremos argumentos.
			NULL		
		) != 0
	){
		/*
		Caso seja uma criação successful, o número correspondente da thread vai parar o endereço
		que foi dado no primeiro argumento.
		
		Retornos Possíveis para pthread_create:
		
		0 -> Completada Com Sucesso;
		EAGAIN -> Muitas Threads foram criadas;
		EINVAL -> Atributos dados inválidos;
		EPERM -> Agasalha o erro.
		*/
		return 1;
	}
	
	pthread_create(
		&exemplo_2,
		NULL,
		funcao_a_ser_assincrona,
		NULL
	);
	
	//////////////////////////////////////////////////////////////////////////////////
	
	void *valor_a_ser_retornado_pela_funcao_executada = NULL;
	
	/*
	Bloqueia execução esperando a thread específica terminar.
	*/
	if(
		pthread_join(
			// Não mais o endereço, mas o valor.
			exemplo_de_thread_1,
			
			// Local onde guardaremos o retorno.
			&valor_a_ser_retornado_pela_funcao_executada
		) != 0
	){
		/*
		Retornos Possíveis para pthread_join:
		
		0 -> Com Sucesso;
		ESRCH -> ID de thread não foi encontrada.
		...
		*/
	}

	pthread_join(
		exemplo_2,
		NULL
	);
	
	return 0;
}







