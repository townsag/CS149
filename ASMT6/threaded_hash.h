#ifndef THREADED_HASH_H
#define THREADED_HASH_H

#include <pthread.h>

#define HASH_SIZE 10

typedef struct __node_t {
        /* table entry: */
        struct __node_t *next; /* next entry in chain */
        char* name;
	int count;
} node_t;

typedef struct __list_t {
	node_t* head;
	pthread_mutex_t list_lock;
} list_t;

typedef struct __threaded_hash_table {
	list_t** table;
} threaded_hash_table_t;


node_t* new_node(char* name);
void print_node(node_t* node);

list_t* new_list();
void print_list(list_t* list);
void free_list_recursive(list_t* head);
int insert_list(list_t* list, char* name);
int search_List(list_t* list, char* name);
int increment_name(list_t* list, char* name);
//int search_and_add_or_increment(list_t* list, char* name);

threaded_hash_table_t* new_threaded_hash_table();
unsigned hash(char* name);
int lookup_name(threaded_hash_table_t* table_obj, char* name);
int add_name(threaded_hash_table_t* table_obj, char* name);
void print_hash_table(threaded_hash_table_t* table_obj);
void pretty_print(threaded_hash_table_t* table_obj);
void free_hash_table(threaded_hash_table_t* to_free);

#endif
