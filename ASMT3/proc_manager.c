#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define LINE_LENGTH 30
#define MAX_COMMANDS 100

void initialize_array(char commands[MAX_COMMANDS][LINE_LENGTH]){
	//initialize the strings in array to zero
	for(int i = 0; i < MAX_COMMANDS; i++){
		for(int j = 0; j < LINE_LENGTH; j++){
			commands[i][j] = '\0';
		}
	}
}


int main(int argc, char* argv[]){
	char commands[MAX_COMMANDS][LINE_LENGTH];
	int num_commands = 0;
	initialize_array(commands);

	char buffer[LINE_LENGTH];
	while(fgets(buffer, LINE_LENGTH, stdin) != NULL){
		if(buffer[strlen(buffer) - 1] == '\n'){
			buffer[strlen(buffer) - 1] = '\0';
		}
		strcpy(commands[num_commands], buffer);
		num_commands++;
	}

	pid_t pid;
	for(int i = 0; i < num_commands; i++){
		pid = fork();
		if(pid == 0){	//in the child process
			//change the stdout and stderr files to be pid.out and pid.err
			pid_t child_pid = getpid();
			char file_name_str[20];
		        sprintf(file_name_str, "%d.txt", child_pid);	
			int out_file_desc = open(file_name_str, O_RDWR | O_CREAT | O_APPEND, 0777);
			dup2(out_file_desc, STDOUT_FILENO);
			printf("%s, %d\n", commands[i], getpid());
			exit(0);
		} else if (pid < 0){
			fprintf(stderr,"fork failed\n");
		}
	
	}


	return 0;
}
