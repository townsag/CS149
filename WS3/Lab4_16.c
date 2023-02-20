#include <stdio.h>

int main(void) {

	//input any number of non negative numbers then stop on the negative number (dont include negative)
	
	int nums[50];
	int last_num = 0;
	int running_max = 0;
	int num_nums = 0;

	for(int i = 0; i < 50; i++){
		nums[i] = 0;
	}

	scanf("%d", &last_num);
	while(last_num >= 0){
		if(last_num > running_max){
			running_max = last_num;
		}
		nums[num_nums] = last_num;
		num_nums++;
		scanf("%d", &last_num); 
	}

	printf("%d ", running_max);
	int sum = 0;
	for(int i = 0; i < num_nums; i++){
		sum += nums[i];
	}
	printf("%.2f\n",(double) sum / num_nums);
	return 0;
}

