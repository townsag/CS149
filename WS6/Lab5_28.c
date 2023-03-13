#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(void){

	char name[50];
	char name_no_space[50];
	fgets(name, 50, stdin);
	
	if(name[strlen(name) - 1] == '\n'){
		name[strlen(name) - 1] = '\0';
	}

	int count = 0;
	for(int i = 0; i < strlen(name); i++){
		if(name[i] != ' '){
			name_no_space[count++] = name[i];
		}
	}

	printf("%s\n", name);
	printf("%s\n", name_no_space);

	int head = 0;
	int tail = strlen(name_no_space) - 1;
	while(head < tail ){
		printf("head: %c, tail: %c\n", name_no_space[head], name_no_space[tail]);
		if(name_no_space[head] == name_no_space[tail]){
			head++; tail--;
		} else {
			printf("not a palindrome: %s\n", name);
			exit(0);
		}
	}
	printf("palindrome: %s\n", name);
	exit(0);
}

