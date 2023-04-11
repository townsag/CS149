#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
//#include <sys/wait.h>

#include "from_template.h"

#define INITIAL_BUFFER_SIZE 10;

//This function reads in a line from a file (or stdin) and then returns a pointer to that input
//because this function allocates memory, it is the job of the caller to deallocate that memory
//this function makes no assumtions about the size of the line, only that the input file is reachable
//this function returns a NULL pointer if there is some sort of failure or if the EOF is reached
char* read_and_allocate(FILE* input){
	
	PUSH_TRACE("read_and_allocate");

	//printf("entering RAA\n");
	size_t buffer_size = INITIAL_BUFFER_SIZE;
	char* buffer = (char *)malloc(buffer_size*sizeof(char));

	if(buffer == NULL){
		printf("failed to allocate buffer\n");
		return NULL;
	}
	
	char c;
	size_t index_to_write = 0;
	
	c = fgetc(input);
	while(c != '\n' && !feof(input)){
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
		c = fgetc(input);
	}
	buffer[index_to_write] = '\0';

	//check for empty buffer and EOF case
	if(index_to_write == 0 && feof(input)){
		printf("reached EOF logic\n");
		free(buffer);
		return NULL;
	}
		
	//printf("leaving RAA\n");
	POP_TRACE();

	return buffer;
}

struct my_node{
	char* line;
	int line_index;
	struct my_node * next;
};

//this function recursively frees a linked list
//this function makes the assumption that the head of the linked list is not a null pointer
//this function does not return anything
void free_list(struct my_node* head){
	PUSH_TRACE("free_list");
	
	/*
	struct my_node* temp = head;
	struct my_node* temp_look_ahead = head->next;

	while(temp != NULL){
		free(temp->line);
		free(temp);
		temp = temp_look_ahead;
		if(temp != NULL){
			temp_look_ahead = temp->next;
		}
	}
	
	POP_TRACE();	
	*/
	
	struct my_node * temp = head->next;
	free(head->line);
	free(head);
	if(temp != NULL){
		free_list(temp);
	}
	POP_TRACE();
}

//this function prints the contents of a linked list
//this function makes very few assumptions, for example it can also print a null pointer head linked list
//this function does not return anything
void print_list(struct my_node* head){
	PUSH_TRACE("print_list");

	struct my_node* temp = head;
        while(temp != NULL){
		printf("Line %d: %s\n", temp->line_index, temp->line);
		temp = temp->next;
	}

	//printf("Line %d: %s\n", head->line_index, head->line);
	//if(head->next != NULL){
	//	print_list(head->next);
	//}
	POP_TRACE();
}


//This is the main function in which I call all the other functions
//this function is used to allocate memory for the list of commands and the iteratively call the function which
//returns a pointer to new memory holding a command
//This function is responsible for calling the funtions which deallocate memory or for deallocating memory itself
//this function does not return anything but it does print a number of times
int main(int argc, char* argv[]){
	PUSH_TRACE("main");

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
	
	POP_TRACE();

	return 0;
}






