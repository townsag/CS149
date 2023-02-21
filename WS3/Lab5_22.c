#include <stdio.h>

int main(){
	int nums[20];
	int num_input;
	int min;
	int max;

	scanf("%d", &num_input);
	for(int i = 0; i < num_input; i++){
		scanf("%d", &nums[i]);
	}
	scanf("%d", &min);
	scanf("%d", &max);

	for(int i = 0; i < num_input; i++){
		if(min <= nums[i] && nums[i] <= max){
			printf("%d,", nums[i]);
		}
	}
	printf("\n");



	return(0);
}
