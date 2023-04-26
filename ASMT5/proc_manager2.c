#define _GNU_SOURCE 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "utils.h"
#include "hash.h"

void write_exit_code(pid_t child_pid, pid_t parent_pid, int status, struct nlist* node){
	char* out_file_name_str = NULL;
	char* err_file_name_str = NULL;
	asprintf(&out_file_name_str, "%d.out", child_pid);
	asprintf(&err_file_name_str, "%d.err", child_pid);
	
	int out_file_desc = open(out_file_name_str, O_WRONLY | O_APPEND, 0777);
	int err_file_desc = open(out_file_name_str, O_WRONLY | O_APPEND, 0777);
	if(out_file_desc == -1 || err_file_desc == -1){
		free(out_file_name_str);
		free(err_file_name_str);
		perror("failed to open\n");
		return;
	}
	
	char* out_message = NULL;
	asprintf(&out_message, "Finished child %d pid of parent %d\n Finished at %f, runtime duration %f\n", child_pid, parent_pid, node->start.tv_sec, get_duration(node->start));
	int len_written = write(out_file_desc, out_message, strlen(out_message));
	if(len_written == -1){
		perror("failed to print\n");
		return;
	}
	free(out_message);

	if(WIFSIGNALED(status)){
		char* message = NULL;
		asprintf(&message, "killed with signal %d\n", WTERMSIG(status));
		int len_written = write(err_file_desc, message, strlen(message));
                if(len_written == -1){
                        perror("failed to print\n");
			free(message);
			free(out_file_name_str);
			free(err_file_name_str);
                        return;
                }
		free(message);

	} else {
		char* message = NULL;
		asprintf(&message, "Exited with exitcode = %d\n", WEXITSTATUS(status));
		int len_written = write(out_file_desc, message, strlen(message));
		if(len_written == -1){
			free(message);
			free(out_file_name_str);
			free(err_file_name_str);
			perror("failed to print\n");
			return;
		}
		free(message);
	}
	free(out_file_name_str);
	free(err_file_name_str);
	close(out_file_desc);
	return;
}

//This is the main function in which I call all the other functions
//this function is used to allocate memory for the list of commands and the iteratively call the function which
//returns a pointer to new memory holding a command
//This function is responsible for calling the funtions which deallocate memory or for deallocating memory itself
//this function does not return anything but it does print a number of times
int main(int argc, char* argv[]){

	//int out_file_desc = open("memtrace.out", O_RDWR | O_CREAT | O_APPEND, 0777);
	//dup2(out_file_desc, STDOUT_FILENO);

	struct my_commands* commands_obj = new_commands();
	if(commands_obj == NULL){
		printf("failed to allocate commands\n");
		return 1;
	}

	struct my_list* list_obj = new_my_list();
	if(list_obj == NULL){
		printf("failed to allocate list\n");
		return 1;
	}
	
	struct hash_table* hash_obj = new_hash_table();
	if(hash_obj == NULL){
		printf("failed to allocate hash table\n");
		return 1;
	}

	printf("starting while loop\n");
	char* temp_str = NULL;
	int counter = 0;
	while((temp_str = read_and_allocate(stdin)) != NULL){
		//the commands list gets temp str and the linked list gets copy_str
                char* copy_str = strdup(temp_str);
		append_node(list_obj, new_node(copy_str, counter++));
		
		//memory allocated by read_and_allocate is sent to commands struct, commands struct
		//is responsible for freeing this memory, freed in free_commands
		int status = add_command(commands_obj, temp_str);
		if(status == 1){
			printf("failed to add to commands\n");
			return 1;
		}
		temp_str = NULL;
	}
	printf("finished while loop\n");
	printf("printing all of commands list\n");
	print_commands(commands_obj);

	printf("printing linked list of nodes\n");
	print_my_list(list_obj);
	printf("finished printing linked list\n");	

	pid_t pid;
	int num_children = 0;
	for(int index = 0; index < commands_obj->num_commands; index++){
		pid = fork();
		if(pid == 0){//in the child process
			//change the stdout and stderr files to be pid.out and pid.err
			pid_t child_pid = getpid();
			pid_t parent_pid = getppid();

			char *err_filename = NULL;
    			char *out_filename = NULL;
    			asprintf(&err_filename, "%d.err", child_pid);
			asprintf(&out_filename, "%d.out", child_pid);
    			int err_fd = open(err_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    			int out_fd = open(out_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    			
			if (err_fd == -1 || out_fd == -1) {
		        	perror("open");
			        exit(EXIT_FAILURE);
			}
			
			if (dup2(out_fd, STDOUT_FILENO) == -1 || dup2(err_fd, STDERR_FILENO) == -1) {
				perror("dup2");
				exit(EXIT_FAILURE);
		    	}

			//store the relevant information for the process in the hash table
			struct nlist* temp = insert(hash_obj, child_pid, strdup(commands_obj-commands[index]->command), index);
			add_start(temp);

			printf("Starting command %i: child %d pid of parent %d\n", index, child_pid, parent_pid);
			fflush(stdout);
			
			execvp(commands_obj->commands[index]->command_split[0], commands_obj->commands[index]->command_split);
			printf("couldn't execute: %s", commands_obj->commands[index]->command);
			exit(2);
		} else if (pid < 0){
			fprintf(stderr,"fork failed\n");
		} else {
			num_children++;
		}
	}

	int status;
	pid_t exited_pid;
	
	printf("starting second while loop, num_children: %d\n", num_children);
	while(num_children > 0){
		exited_pid = waitpid(-1, &status, WNOHANG);
        	if(exited_pid == -1) { // check for errors
            		perror("waitpid");
        	} else if (exited_pid > 0) { // child process finished
            		num_children--;
			struct nlist* temp_node = lookup(table_obj, exited_pid);
			if(temp_node == NULL){
				printf("pid not found\n");
				return 1;
			}
			add_stop(temp_node);
			write_exit_code(exited_pid, getpid(), status, temp_node);
			//write_exit_code(exited_pid, WEXITSTATUS(status));
        	}
    	}

	printf("ending second while loop\n");

	//free memory for strings in commands, commands list, strings in linked list, linked list
	free_commands_struct(commands_obj);
	//free_list(head);
	free_my_list(list_obj);
	free_hash_table(hash_obj);
	return 0;
}
