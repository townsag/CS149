#ifndef HASH_H
#define HASH_H

#include <time.h>

#define HASH_SIZE 101

struct nlist {
        /* table entry: */
        struct nlist *next; /* next entry in chain */
        char* command;
        struct timespec start;
	struct timespec stop;
	int index;
	pid_t pid;
	int respawn_flag;	//1: nothing, 0:spawing too fast
        /* index // this is the line index in the input text file */
        /* pid // the process id. you can use the pid result of wait to lookup in the hashtable */
        /* char *command; // command. This is good to store for when you decide to restart a command */
};

struct hash_table {
	struct nlist** table;
};

struct nlist* new_nlist(pid_t pid, char* command, int index);
void set_start(struct nlist* to_add, struct timespec temp_start);
void add_start(struct nlist* to_start);
void add_stop(struct nlist* to_stop);
double get_duration(struct nlist* node);
void print_node(struct nlist* node);
void print_nlist(struct nlist* head);
void free_nlist(struct nlist* to_free);
void free_nlist_recursive(struct nlist* head);

struct hash_table* new_hash_table();
unsigned hash(pid_t pid);
struct nlist* lookup(struct hash_table* table_obj, pid_t pid);
struct nlist* insert(struct hash_table* table_obj, pid_t pid, char* command, int index);
void print_hash_table(struct hash_table* table_obj);
void free_hash_table(struct hash_table* to_free);

#endif
