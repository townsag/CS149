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

	struct my_commands* commands_obj = new_commands();
	if(commands_obj == NULL){
		printf("failed to allocate commands\n");
		return 1;
	}

	struct my_node* head = (struct my_node*) malloc(sizeof(struct my_node));
	head->next = NULL;
	struct my_node* tail = head;

	printf("starting while loop\n");
	char* temp_str = NULL;
	while((temp_str = read_and_allocate(stdin)) != NULL){
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
		
		//memory allocated by read_and_allocate is sent to commands struct, commands struct
		//is responsible for freeing this memory, freed in free_commands
		int status = add_command(command_obj, temp_str);
		if(status == 1){
			printf("failed to add to commands\n");
			return 1;
		temp_str = NULL;
	}
	printf("finished while loop\n");
	
	//housekeeping, remove default head from linked list
	struct my_node * temp_head = head;
	head = head->next;
	temp_head->next = NULL;
	free_list(temp_head);
	
	printf("printing all of commands list\n");
	print_commands(commands_obj);

	printf("printing all of linked list\n");
	if(head != NULL){
		print_list(head);
	}
	
	//free memory for strings in commands, commands list, strings in linked list, linked list
	free_commands_struct(commands_obj);
	free_list(head);

	return 0;
}
