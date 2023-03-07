#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void){
	fork();
	fork();
	printf("hello world from process %d\n", getpid());
	return 0;
}

	
