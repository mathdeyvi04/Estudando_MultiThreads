#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void*
funcao_a_ser_assincrona(){
	/*
	Executada pela thread que a chamar. Aten��o ao tipo.
	
	Caso n�o fosse ass�ncrono, a sa�da seria:
		
		Comecei uma thread.
		Encerrei uma thread.
		Comecei uma thread.
		Encerrei uma thread.
		
	Mas sabemos que n�o � isso.
	
	ELAS COMPARTILHAM A MESMA MEM�RIA.
 	*/
	
	printf("Comecei uma thread.\n");
	
	sleep(1);
	
	printf("\nEncerrei uma thread.\n");
	
}

int main(){
	// Vari�vel que armazenar� a informa��o da thread.
	pthread_t exemplo_de_thread_1;
	pthread_t exemplo_2;
	
	
	if (
		pthread_create(
		
			// Endere�o de mem�ria da vari�vel que armazenar� a thread.
			&exemplo_de_thread_1,
			
			// Default
			NULL,
			
			
			/*
			-------------------------------------PARTE MAIS COMPLEXA--------------------------
			-> void *(*func)(void*)
			
			void *
				O tipo de retorno da fun��o � gen�rico. Isso significa que pode ser convertido
				para qualquer tipo, desde que feito corretamente.
			
			(*func)
				Indica que 'func' � o ponteiro para a fun��o, n�o a fun��o diretamente.
				Isto significa que deve ser passado o endere�o de mem�ria da fun��o.
			
			(void*)
				Indica que a fun��o apontada por 'func' aceita um �nico argumento,
				que � um ponteiro gen�rico.
			*/
			funcao_a_ser_assincrona,
			
			// Por agora, n�o colocaremos argumentos.
			NULL		
		) != 0
	){
		/*
		Caso seja uma cria��o successful, o n�mero correspondente da thread vai parar o endere�o
		que foi dado no primeiro argumento.
		
		Retornos Poss�veis para pthread_create:
		
		0 -> Completada Com Sucesso;
		EAGAIN -> Muitas Threads foram criadas;
		EINVAL -> Atributos dados inv�lidos;
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
	Bloqueia execu��o esperando a thread espec�fica terminar.
	*/
	if(
		pthread_join(
			// N�o mais o endere�o, mas o valor.
			exemplo_de_thread_1,
			
			// Local onde guardaremos o retorno.
			&valor_a_ser_retornado_pela_funcao_executada
		) != 0
	){
		/*
		Retornos Poss�veis para pthread_join:
		
		0 -> Com Sucesso;
		ESRCH -> ID de thread n�o foi encontrada.
		...
		*/
	}

	pthread_join(
		exemplo_2,
		NULL
	);
	
	return 0;
}







