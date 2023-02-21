#include <stdio.h>
#include <limits.h>

int main(){

	int nums[20];
	int num_inputs;
	int min1 = INT_MAX;
        int min2 = INT_MAX;

	scanf("%d", &num_inputs);
	for(int i = 0; i < num_inputs; i++){
		scanf("%d", &nums[i]);
		if(nums[i] < min1){
			if(min1 < min2){
				min2 = min1;
			}
			min1 = nums[i];
		} else if (nums[i] < min2){
			min2 = nums[i];
		}
	}
	
	printf("%d and %d\n", min1, min2);

	return(0);
}

