#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <unistd.h>
//#include <fcntl.h>
//#include <sys/wait.h>

#define INITIAL_BUFFER_SIZE 10;

char* read_and_allocate(FILE* input){
	printf("entering RAA\n");
	size_t buffer_size = INITIAL_BUFFER_SIZE;
	char* buffer = (char *)calloc(buffer_size, sizeof(char));

	if(buffer == NULL){
		printf("failed to allocate buffer\n");
		return NULL;
	}

	char c;
	size_t index_to_write = 0;

	while((c = fgetc(input)) != '\n' && c!= EOF){
		//check to see if the buffer is full
		if(index_to_write == buffer_size - 2){	//leave room for null at end of string
			//resize the buffer
			buffer_size *= 2;
			char* temp = (char*) realloc(buffer, buffer_size * sizeof(char));
			if(temp == NULL){
				printf("failed to reallocate memory\n");
				free(buffer);
				return NULL;
			}
			buffer = temp;
		}
		buffer[index_to_write++] = c;
	}
	buffer[index_to_write] = '\0';

	//check for empty buffer and EOF case
	if(index_to_write == 0 && c == EOF){
		printf("reached EOF logic\n");
		return NULL;
	}
		
	printf("leaving RAA\n");

	return buffer;
}



int main(int argc, char* argv[]){
	char** commands = NULL;
	int num_commands = 0;
	int commands_size = INITIAL_BUFFER_SIZE;

	commands = (char**) calloc(commands_size, sizeof(char*));
	if(commands == NULL){
		printf("failed to allocate commands\n");
		return(1);
	}

	printf("starting while loop\n");
	char* temp_str = NULL;
	while((temp_str = read_and_allocate(stdin)) != NULL){
		//check to see if commands is full
		if(num_commands == commands_size){
			//reallocate commands
			commands_size *= 2;
			char** temp = (char**) realloc(commands, commands_size * sizeof(char*));
			if(temp == NULL){
				printf("failed to reallocate commands\n");
				free(commands);
				if(temp_str != NULL){
					free(temp_str);
				}
			}
			commands = temp;
		}
		commands[num_commands++] = temp_str;
		printf("you entered: %s\n", commands[num_commands - 1]);
		temp_str = NULL;
	}
	printf("finished while loop\n");

	for(int i = 0; i < num_commands; i++){
		printf("line %d: %s\n", i, commands[i]);
	}

	for(int i = 0; i < num_commands; i++){
		free(commands[i]);
	}
	free(commands);
	
	return 0;
}






