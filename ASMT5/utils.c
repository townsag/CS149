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

/*
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
}*/

struct my_node* new_node(char* line, int index){
	struct my_node* temp = (struct my_node*)calloc(1, sizeof(struct my_node));
	if(temp == NULL){
		printf("failed to allocate node\n");
		return NULL;
	}
	temp->line = line;
	temp->line_index = index;
	return temp;
}

void free_my_node(struct my_node* to_free){
	free(to_free->line);
	free(to_free);
}

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

struct one_command* new_one_command(char* input_str){
	struct one_command* temp = (struct one_command*)calloc(1, sizeof(struct one_command));
	temp->command = input_str;
	//temp->num_tokens = count_tokens(input_str, ' ');
	temp->command_split = NULL;
	const char delim[2] = " ";

	char* copy_str = strdup(input_str);
	char* token = strtok(copy_str, delim);
	int i = 0;

	while(token != NULL){
		temp->command_split = (char**)realloc(temp->command_split, sizeof(char*) * (i + 1));
		int temp_len = strlen(token) + 1;
		temp->command_split[i] = malloc(temp_len);
		strcpy(temp->command_split[i], token);
		i++;
		token = strtok(NULL, delim);
	}
	
	//add null to the end of the command split string list
	temp->command_split = (char**)realloc(temp->command_split, sizeof(char*) * (i + 1));
	temp->command_split[i++] = NULL;
	temp->num_tokens = i;
	free(copy_str);
	return temp;
}

void print_one_command(struct one_command* to_print){
	printf("%s\n", to_print->command);
	for(int i = 0; i < to_print->num_tokens - 1; i++){
		printf("%s\n", to_print->command_split[i]);
	}
}

void free_one_command(struct one_command* to_free){
	free(to_free->command);
	for(int i = 0; i < to_free->num_tokens - 1; i++){	//last token should be null
		free(to_free->command_split[i]);
	}
	free(to_free->command_split);
	free(to_free);
}
	
struct my_commands* new_commands (){
	struct my_commands* temp = (struct my_commands*)calloc(1, sizeof(struct my_commands));
	temp->size = INITIAL_BUFFER_SIZE;
	temp->num_commands = 0;
	temp->commands = (struct one_command**)calloc(temp->size, sizeof(struct one_command*));
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
                struct one_command** temp = (struct one_command**) realloc(commands_struct->commands, commands_struct->size * sizeof(struct one_command*));
                if(temp == NULL){
			printf("failed to reallocate commands\n");
                        commands_struct->size /= 2;
                        return 1;
		}
		commands_struct->commands = temp;
	}
	
	//commands_struct->commands[commands_struct->num_commands++] = command_str;
	commands_struct->commands[commands_struct->num_commands++] = new_one_command(command_str);
	
	return 0;
}

struct one_command* get_command(struct my_commands* commands_struct, int index){
	if(index >= commands_struct->num_commands){
		printf("index out of bounds in get command\n");
		return NULL;
	}
	
	return commands_struct->commands[index];
}

void print_commands(struct my_commands* commands_struct){
	for(int i = 0; i < commands_struct->num_commands; i++){
		//printf("%s\n", commands_struct->commands[i]);
		print_one_command(commands_struct->commands[i]);
	}
}

void free_commands_struct(struct my_commands* commands_struct){
	for(int i = 0; i < commands_struct->num_commands; i++){
		free_one_command(commands_struct->commands[i]);
	}
	free(commands_struct->commands);
	free(commands_struct);
}


struct my_list* new_my_list(){
	struct my_list* temp = (struct my_list*)calloc(1, sizeof(struct my_list));
	if(temp == NULL){
		printf("failed to allocate new list\n");
		return NULL;
	}
	temp->head = NULL;
	temp->tail = NULL;
	return temp;
}

void append_node(struct my_list* list, struct my_node* to_add){
	if(list->tail == NULL){
		list->head = list-> tail = to_add;
	} else {
		list->tail->next = to_add;
		list->tail = to_add;
	}
}

void free_my_list(struct my_list* to_free){
	struct my_node* temp = to_free->head;

	while(temp != NULL){
		struct my_node* look_ahead = temp->next;
		//free(temp->line);
		//free(temp);
		free_my_node(temp);
		temp = look_ahead;
	}
	free(to_free);
}

void print_my_list(struct my_list* to_print){
	struct my_node* temp = to_print->head;
        while(temp != NULL){
                printf("Line %d: %s\n", temp->line_index, temp->line);
                temp = temp->next;
        }
}
