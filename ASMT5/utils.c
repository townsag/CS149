#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils.h"


//This function reads in a line from a file (or stdin) and then returns a pointer to that input
//because this function allocates memory, it is the job of the caller to deallocate that memory
//this function makes no assumtions about the size of the line, only that the input file is reachable
//this function returns a NULL pointer if there is some sort of failure or if the EOF is reached
char* read_and_allocate(FILE* input){

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
                if(index_to_write == buffer_size - 2){  //leave room for null at end of string
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

        return buffer;
}

//this function recursively frees a linked list
//this function makes the assumption that the head of the linked list is not a null pointer
//this function does not return anything
void free_list(struct my_node* head){

        struct my_node * temp = head->next;
        free(head->line);
        free(head);
        if(temp != NULL){
                free_list(temp);
        }
//this function prints the contents of a linked list
//this function makes very few assumptions, for example it can also print a null pointer head linked list
//this function does not return anything
void print_list(struct my_node* head){

        struct my_node* temp = head;
        while(temp != NULL){
                printf("Line %d: %s\n", temp->line_index, temp->line);
                temp = temp->next;
        }
}


struct my_commands* new_commands (){
	struct my_commands* temp = (struct my_commands*)calloc(sizeof(struct my_commands));
	temp->size = INITIAL_BUFFER_SIZE;
	temp->num_commands = 0;
	temp->commands = (char**)calloc(temp->size, sizeof(char**));
	if(temp->commands == NULL){
		printf("failed to allocate commands\n");
		return NULL;
	}
	return temp;
}

int add_command(struct my_commands* commands_struct, char* command_str){
	//check to see if commands is full
	if(commands_struct->num_commands == commands_struct->size){
     		//reallocate commands
                commands_struct->size *= 2;
                char** temp = (char**) realloc(commands_struct->commands, commands_struct->size * sizeof(char*));
                if(temp == NULL){
			printf("failed to reallocate commands\n");
                        commands_struct->size /= 2;
                        return 1;
		}
		commands = temp;
	}
	
	commands_struct->commands[commands_struct->num_commands++] = command_str;
	return 0;
}

char* get_command(struct my_commands* commands_struct, int index){
	if(index >= commands_struct->num_commands){
		printf("index out of bounds in get command\n");
		return NULL;
	}
	
	return commands_struct->commands[index];
}

int free_commands_struct(struct my_commands* commands_struct){
	for(int i = 0; i < commands_struct->num_commands; i++){
		free(commands_struct->commands[i]);
	}
	free(commands_struct->commands)
	free(commands_struct);
}



