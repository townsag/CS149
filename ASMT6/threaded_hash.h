#ifndef THREADED_HASH_H
#define THREADED_HASH_H

#include <time.h>

#define HASH_SIZE 101

struct nlist {
        /* table entry: */
        struct nlist *next; /* next entry in chain */
        char* name;
	int count;
};

struct threaded_hash_table {
	struct nlist** table;
};

struct nlist* new_nlist(char* name);
void print_node(struct nlist* node);
void print_nlist(struct nlist* head);
void free_nlist(struct nlist* to_free);
void free_nlist_recursive(struct nlist* head);

struct threaded_hash_table* new_threaded_hash_table();
unsigned hash(char* name);
struct nlist* lookup(struct threaded_hash_table* table_obj, char* name);
struct nlist* insert(struct threaded_hash_table* table_obj, char* name);
void print_hash_table(struct threaded_hash_table* table_obj);
void free_hash_table(struct threaded_hash_table* to_free);

int get_count(struct theaded_hash_table* table_obj, char* name);

#endif
