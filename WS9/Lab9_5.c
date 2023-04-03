#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 30


char* read_names(){
	printf("Enter input string:\n");
	char* new_string = (char*)calloc(MAX_LENGTH, sizeof(char));
	fgets(new_string, MAX_LENGTH, stdin);
	return new_string;
}

char* remove_spaces(char* front){
	//remove from the front first
	char* temp = front;
	while(*temp == ' '){
		temp++;
	}
	//remove from the end next
	while(temp[strlen(temp) - 1] == ' '){
		temp[strlen(temp) - 1] = '\0';
	}
	return temp;
}


int main(void){
	char* names;
	while(strcmp((names = read_names()),"q\n")){
		char* comma_pointer = strchr(names, ',');
		if(comma_pointer == NULL){
			printf("Error: No comma in string.\n\n");
		} else {
			//remove the endline from the string
			if(names[strlen(names) - 1] == '\n'){
				names[strlen(names) - 1] = '\0';
			}
			*comma_pointer = '\0';
			char* first_word = remove_spaces(names);
			char* second_word = remove_spaces(comma_pointer + 1);
			printf("First word: %s\n", first_word);
			printf("Second word: %s\n\n", second_word);
		}
		free(names);
	}
	free(names);
	return 0;
}
