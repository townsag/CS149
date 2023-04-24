#ifndef UTILS_H
#define UTILS_H

#define INITIAL_BUFFER_SIZE 10;

char* read_and_allocate(FILE* in);

struct my_node{
        char* line;
        int line_index;
        struct my_node * next;
};

struct my_commands{
	char** commands;
	int num_commands;
	int size;
};

struct my_commands* new_commands();
void add_command(struct my_commands* commands, char* command_str);
char* get_command(struct my_commands* commands, int index);
void free_commands_struct(struct my_commands* commands);


void free_list(struct my_node* head);
void print_list(struct my_node* head);

#endif
