#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int CalcNumCharacters(char* userString, char userChar){
	int count = 0;
	for(int i = 0; i < strlen(userString); i++){
		if(userString[i] == userChar){
			count++;
		}
	}
	return count;
}


int main(void){
	char userString[50];
	char userChar;
	scanf("%c ", &userChar);
	fgets(userString, 50, stdin);

	if(userString[strlen(userString) - 1] == '\n'){
		userString[strlen(userString) - 1] = '\0';
	}

	int count = CalcNumCharacters(userString, userChar);

	if(count == 1){
		printf("1 %c\n", userChar);
	} else {
		printf("%d %c's\n", count, userChar);
	}

	return 0;


}
