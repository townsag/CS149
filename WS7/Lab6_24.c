#include <stdio.h>
#include <stdlib.h>

void SwapValues(int* userVal1, int* userVal2, int* userVal3, int* userVal4){
	int temp1 = *userVal1;
	int temp2 = *userVal3;
	*userVal1 = *userVal2;
	*userVal2 = temp1;
	*userVal3 = *userVal4;
	*userVal4 = temp2;
}


int main(void){
	int* temp1 = malloc(sizeof(int));
	int* temp2 = malloc(sizeof(int));
	int* temp3 = malloc(sizeof(int));
	int* temp4 = malloc(sizeof(int));

	scanf("%d %d %d %d", temp1, temp2, temp3, temp4);
	SwapValues(temp1, temp2, temp3, temp4);
	printf("%d %d %d %d\n", *temp1, *temp2, *temp3, *temp4);
	return 0;
}
