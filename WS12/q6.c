#include <stdlib.h>
#include <stdio.h>

int main(void){
	int* array = (int*) malloc(5*sizeof(int));
	array[0] = 20;
	free(array);
	printf("%d\n", array[0]);
	return 0;
}
