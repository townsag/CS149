#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
// #include <sys/wait.h>

#include "utils.h"


//This is the main function in which I call all the other functions
//this function is used to allocate memory for the list of commands and the iteratively call the function which
//returns a pointer to new memory holding a command
//This function is responsible for calling the funtions which deallocate memory or for deallocating memory itself
//this function does not return anything but it does print a number of times
int main(int argc, char* argv[]){

	int out_file_desc = open("memtrace.out", O_RDWR | O_CREAT | O_APPEND, 0777);
	dup2(out_file_desc, STDOUT_FILENO);

	char** commands = NULL;
	int num_commands = 0;
	int commands_size = INITIAL_BUFFER_SIZE;

	struct my_node* head = (struct my_node*) malloc(sizeof(struct my_node));
	head->next = NULL;
	struct my_node* tail = head;

	commands = (char**) malloc(commands_size * sizeof(char*));
	if(commands == NULL){
		printf("failed to allocate commands\n");
		free_list(head);
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
				for(int i = 0; i < num_commands; i++){
					free(commands[i]);
				}
				free(temp_str);
				free(commands);
				free_list(head);
				return 1;
			}
			commands = temp;
		}

		//add the line to the linked list
                //allocate new memory for the string pointed to by each node
                //should be simpler when deallocating
                struct my_node* temp_node = (struct my_node *)malloc(1* sizeof(struct my_node));
                temp_node->line = (char *)malloc((strlen(temp_str) + 1)* sizeof(char));
                strcpy(temp_node->line, temp_str);
                temp_node->line_index = num_commands;
                temp_node->next = NULL;
                tail->next = temp_node;
                tail = temp_node;
		
		commands[num_commands++] = temp_str;
		//printf("you entered: %s\n", commands[num_commands - 1]);
		temp_str = NULL;
	}
	printf("finished while loop\n");
	
	//housekeeping, remove default head from linked list
	struct my_node * temp_head = head;
	head = head->next;
	temp_head->next = NULL;
	free_list(temp_head);

	for(int i = 0; i < num_commands; i++){
		printf("line %d: %s\n", i, commands[i]);
	}

	if(head != NULL){
		print_list(head);
	}
	
	//free memory for strings in commands, commands list, strings in linked list, linked list
	for(int i = 0; i < num_commands; i++){
		free(commands[i]);
	}
	free(commands);
	free_list(head);

	return 0;
}
