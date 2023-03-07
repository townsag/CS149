#include <stdio.h>
#include <string.h>
#include <ctype.h>


void RemoveNonAlpha(char userString[], char userStringAlphaOnly[]){
	int read = 0;
	int write = 0;
	int len = strlen(userString);
	for(int i = 0; i < len; i++){
		userStringAlphaOnly[i] = '\0';
	}
	for(int i = 0; i < len; i++){
		if(isalpha(userString[i]) || userString[i] == '\n'){
			userStringAlphaOnly[write] = userString[read];
			read++;
			write++;
		} else {
			read++;
		}
	}
	return;
}

int main(void){
	char line[50 + 1];
	char alpha_line[50 + 1];
	fgets(line, 51, stdin);
	RemoveNonAlpha(line, alpha_line);
	printf("%s\n", alpha_line);
	return 0;
}
