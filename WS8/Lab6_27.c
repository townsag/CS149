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

	printf("userchar: %c\n", userChar);
	printf("userSentance: %s\n", userString);
	printf("%d\n", CalcNumCharacters(userString, userChar));
	return 0;


}
