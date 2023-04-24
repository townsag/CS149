#ifndef UTILS_H
#define UTILS_H

#define INITIAL_BUFFER_SIZE 10;

char* read_and_allocate(FILE* in);

struct my_node{
        char* line;
        int line_index;
        struct my_node * next;
};

struct one_command{
	char* command;
	char** command_split;
	int num_tokens;
};

struct my_commands{
	struct one_command** commands;
	int num_commands;
	int size;
};

struct my_list{
	struct my_node* head;
	struct my_node* tail;
};

struct my_node* new_node(char* line, int index);
void free_my_node(struct my_node*);

struct my_commands* new_commands();
int add_command(struct my_commands* commands, char* command_str);
struct one_command* get_command_str(struct my_commands* commands, int index);
void print_commands(struct my_commands* commands);
void free_commands_struct(struct my_commands* commands);

struct my_list* new_my_list();
void append_node(struct my_list* list, struct my_node* to_add);
void free_my_list(struct my_list* to_free);
void print_my_list(struct my_list* to_printC);

//void free_list(struct my_node* head);
//void print_list(struct my_node* head);

#endif
