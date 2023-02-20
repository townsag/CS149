#include <stdio.h>

int main(){
	const int NUM_ELEMENTS = 20;         // Number of input integers
	int userVals[NUM_ELEMENTS];          // Array to hold the user's input integers
	int num_inputs = 0;

	scanf("%d", &num_inputs);
	for(int i = 0; i < num_inputs; i++){
		scanf("%d", &userVals[i]);
	}
	for(int i = num_inputs - 1; i >= 0; i--){
		printf("%d,", userVals[i]);
	}
	printf("\n");


	

}
