#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

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

void write_exit_code(pid_t child_pid, int status){
	char* out_file_name_str = calloc(20, sizeof(char));                
	sprintf(out_file_name_str, "%d.err", child_pid);               
	int out_file_desc = open(out_file_name_str, O_WRONLY | O_APPEND, 0777);
	if(out_file_desc == -1){
		perror("failed to open\n");
		return;
	}
	
	if(WIFSIGNALED(status)){
		char buffer[40];
		sprintf(buffer, "Killed with signal %d\n", WTERMSIG(status));
		int len_written = write(out_file_desc, buffer, strlen(buffer));
                if(len_written == -1){
                        perror("failed to print\n");
                        return;
                }

	} else {
		char buffer[40];
		sprintf(buffer, "Exited with exitcode = %d\n", WEXITSTATUS(status)); 
		int len_written = write(out_file_desc, buffer, strlen(buffer));
		if(len_written == -1){
			perror("failed to print\n");
			return;
		}
	}
	free(out_file_name_str);
	close(out_file_desc);
}


int main(int argc, char* argv[]){
	char** commands = NULL;
	int num_commands = 0;
	int num_children = 0;

	char buffer[LINE_LENGTH];
	while(fgets(buffer, LINE_LENGTH, stdin) != NULL){
		if(buffer[strlen(buffer) - 1] == '\n'){
			buffer[strlen(buffer) - 1] = '\0';
		}

		if(buffer[0] != '\0'){
			char* temp_new_str = calloc(strlen(buffer) + 1, sizeof(char));
			strcpy(temp_new_str, buffer);
			num_commands++;

			commands = realloc(commands, num_commands * sizeof(char*)); // resize array of strings
			commands[num_commands - 1] = temp_new_str;
		}
	}

	pid_t pid;
	for(int i = 0; i < num_commands; i++){
		pid = fork();
		if(pid == 0){	//in the child process
			//change the stdout and stderr files to be pid.out and pid.err
			pid_t child_pid = getpid();
			pid_t parent_pid = getppid();
			char* out_file_name_str = calloc(20, sizeof(char));
			char* err_file_name_str = calloc(20, sizeof(char));
		        sprintf(out_file_name_str, "%d.txt", child_pid);
			sprintf(err_file_name_str, "%d.err", child_pid);
			int out_file_desc = open(out_file_name_str, O_RDWR | O_CREAT | O_APPEND, 0777);
			int err_file_desc = open(err_file_name_str, O_RDWR | O_CREAT | O_APPEND, 0777);
			dup2(out_file_desc, STDOUT_FILENO);
			dup2(err_file_desc, STDERR_FILENO);
			free(out_file_name_str);
			free(err_file_name_str);
			
			printf("Starting command %i: child %d pid of parent %d\n", i, child_pid, parent_pid);
			fflush(stdout);			
			//fprintf(stderr, "printing iside: %d\n", child_pid);
						
			//printf("1, should be commands: %s\n",commands[i]);
			//fflush(stdout);	

			//seperate the string in commands[i] into an array of strings
			char* temp_command = calloc(strlen(commands[i]) + 1, sizeof(char));
			strcpy(temp_command, commands[i]);

			int num_tokens = count_tokens(temp_command, ' ');
			char** args = calloc(num_tokens + 1, sizeof(char*));
			char* temp;
			int count = 0;
			while( (temp = strsep(&temp_command," ")) != NULL ){
				args[count] = temp;
				count++;
			}
			args[count++] = NULL;
			
			execvp(args[0], args);
			printf("couldn't execute: %s", args[0]);
			exit(2);

			/*
			printf("2, count: %d, num_tokens: %d\n", count, num_tokens);
			fflush(stdout);
			//print out all the strings in args
			for(int j = 0; j < num_tokens; j++){
				printf("%s\n", args[j]);
				fflush(stdout);
			}


			

			printf("3, last print: %s, %d\n", commands[i], getpid());
			fflush(stdout);
			*/
			free(args);
			free(temp_command);
			exit(0);
		} else if (pid < 0){
			fprintf(stderr,"fork failed\n");
		} else {
			num_children++;
		}
	}
	
	int status;
	pid_t exited_pid;
	
	while(num_children > 0){
		exited_pid = waitpid(-1, &status, WNOHANG);
        	if (exited_pid == -1) { // check for errors
            		perror("waitpid");
        	} else if (exited_pid > 0) { // child process finished
            		num_children--;
			write_exit_code(exited_pid, status);
			//write_exit_code(exited_pid, WEXITSTATUS(status));
        	}
    	}



	for(int i = 0; i < num_commands; i++){
		free(commands[i]);
	}

	return 0;
}
