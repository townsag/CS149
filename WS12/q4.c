#include <stdlib.h>
#include <stdio.h>

int main(void){

	int* dont_free = (int*)malloc(sizeof(int));
	*dont_free = 4;
	printf("%d\n", *dont_free);
	return 0;
}

