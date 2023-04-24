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

	struct my_list* list_obj = new_my_list();
	if(list_obj == NULL){
		printf("failed to allocate list\n");
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
	
	//free memory for strings in commands, commands list, strings in linked list, linked list
	free_commands_struct(commands_obj);
	//free_list(head);
	free_my_list(list_obj);
	
	return 0;
}
