#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*

pthread_detach is an alternative to pthread_join to reclaim storage for
a thread.

Cases:

	-> Using the attribute:
		Much more difficult and ineligible.
	
	-> Using the pthread_detach(value_of_the_thread):
		Much more easy.

Threads Detaches are not joinable. They only cause error with join, so do not
use for them, just let them kill themselves.

*/


void*
rotina(){
	sleep(1);
	
	printf("\nFinished thread.");
}


int main(){
	
	pthread_t th;
	pthread_create(
		&th,
		NULL,
		rotina,
		NULL
	);
	
	pthread_detach(th);
	
	// Causes error.
	pthread_join(
		th,
		NULL
	);
	
	printf("\nFinished main.");
	pthread_exit(0);
}
