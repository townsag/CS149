#include <stdio.h>
#include <limits.h>

int MaxNumber(int num1, int num2, int num3, int num4){
	int nums[4] = {num1, num2, num3, num4};
	int max = INT_MIN;
	for(int i = 0; i < sizeof(nums)/sizeof(int); i++){
		if(nums[i] > max){
			max = nums[i];
		}
	}
	return max;
}


int MinNumber(int num1, int num2, int num3, int num4){
	int nums[4] = {num1, num2, num3, num4};
        int min = INT_MAX;
        for(int i = 0; i < sizeof(nums)/sizeof(int); i++){
                if(nums[i] < min){
                        min = nums[i];
                }
        }
	return min;

}

int main(void){
	int num1, num2, num3, num4;
	scanf("%d %d %d %d", &num1, &num2, &num3, &num4);
	printf("Maximum is %d\n", MaxNumber(num1, num2, num3, num4));
	printf("Minimum is %d\n", MinNumber(num1, num2, num3, num4));
	return 0;
}
