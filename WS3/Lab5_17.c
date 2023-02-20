#include <stdio.h>
#include <stdlib.h>

int main(void) {
	const int NUM_ELEMENTS = 9;
	int userValues[NUM_ELEMENTS];    // Set of data specified by the user
	int last_num;
	int count_nums = 0;

	scanf("%d", &last_num);
	while(last_num >= 0){
		userValues[count_nums] = last_num;
		if (count_nums > 8 && last_num != -1){
			printf("Too many numbers\n");
			exit(0);
		}
		count_nums++;
		scanf("%d", &last_num);
	}

	printf("Middle item: %d\n", userValues[count_nums / 2]);

	return 0;
}
