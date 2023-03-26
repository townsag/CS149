#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define LINE_LENGTH 30
#define MAX_COMMANDS 100


int count_tokens(char* command, char delim){
	int count = 0;
	if(strlen(command) > 0){ count = 1;}
	for(int i = 0; i < strlen(command); i++){
		if(command[i] == delim){
			count++;
		}
	}
	return count;
}


int main(int argc, char* argv[]){
	//char commands[MAX_COMMANDS][LINE_LENGTH];
	//char** commands = calloc(MAX_COMMANDS * LINE_LENGTH, sizeof(char));
	char** commands = NULL;
	int num_commands = 0;

	char buffer[LINE_LENGTH];
	while(fgets(buffer, LINE_LENGTH, stdin) != NULL){
		if(buffer[strlen(buffer) - 1] == '\n'){
			buffer[strlen(buffer) - 1] = '\0';
		}

		char* temp_new_str = calloc(strlen(buffer) + 1, sizeof(char));
		strcpy(temp_new_str, buffer);
		num_commands++;

		commands = realloc(commands, num_commands * sizeof(char*)); // resize array of strings
		commands[num_commands - 1] = temp_new_str;
	}

	pid_t pid;
	for(int i = 0; i < num_commands; i++){
		pid = fork();
		if(pid == 0){	//in the child process
			//change the stdout and stderr files to be pid.out and pid.err
			pid_t child_pid = getpid();
			char* file_name_str = calloc(20, sizeof(char));
		        sprintf(file_name_str, "%d.txt", child_pid);	
			int out_file_desc = open(file_name_str, O_RDWR | O_CREAT | O_APPEND, 0777);
			free(file_name_str);
			dup2(out_file_desc, STDOUT_FILENO);
			
			/*
			//seperate the string in commands[i] into an array of strings
			int num_tokens = count_tokens(commands[i], ' ');
			char** args = calloc(num_tokens + 1, sizeof(char*));
			char* temp;
			int count = 0;
			while( (temp = strsep(&commands[i]," ")) != NULL ){
				args[count] = temp;
				count++;
			}
			args[count++] = NULL;
			printf("count: %d, num_tokens: %d\n", count, num_tokens);
			//print out all the strings in args
			for(int i = 0; i < num_tokens + 1; i++){
				printf("%s\n", args[i]);
			}*/




			printf("%s, %d\n", commands[i], getpid());
			exit(0);
		} else if (pid < 0){
			fprintf(stderr,"fork failed\n");
		}
	
	}


	return 0;
}
