#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "threaded_hash.h"
#include <string.h>

struct nlist* new_nlist(char* name){
	struct nlist* temp = (struct nlist*)calloc(1, sizeof(struct nlist));
	if(temp == NULL){
		printf("failed to allocate new nlist\n");
		return NULL;
	}
	temp->name = name;
	temp->count = 0;
	temp->next = NULL;
	return temp;
}

void print_node(struct nlist* node){
	printf("Name: %s, count: %d\n", node->name, node->count);
}

void print_nlist(struct nlist* head){
	if(head != NULL){
		print_node(head);
		print_nlist(head->next);
	}
}

void free_nlist(struct nlist* to_free){
	free(to_free->name);
	free(to_free);
}

void free_nlist_recursive(struct nlist* to_free){
	free(to_free->name);
	if(to_free->next != NULL){
		free_nlist_recursive(to_free->next);
	}
	free(to_free);
}

struct threaded_hash_table* new_threaded_hash_table(){
	struct threaded_hash_table* temp = (struct threaded_hash_table*) calloc(1, sizeof(struct threaded_hash_table));
	if(temp == NULL){
		printf("failed to allocate hash table struct\n");
		return NULL;
	}
	
	temp->table = (struct nlist**)calloc(HASH_SIZE, sizeof(struct nlist*));
	if(temp->table == NULL){
		printf("failed to allocate hash table table\n");
		free(temp);
		return NULL;
	}
	for(int i = 0; i < HASH_SIZE; i++){
		temp->table[i] = NULL;
	}
	return temp;
}


/* This is the hash function: form hash value for string s */
unsigned hash(char* name){
	int sum = 0;
	for(int i = 0; i < strlen(name); i++){
		sum += name[i];
	}
	return sum % HASH_SIZE;
}

/* lookup: look for name in hashtab */
/* This is traversing the linked list under a slot of the hash
table. The array position to look in is returned by the hash
function */
struct nlist *lookup(struct threaded_hash_table* table_obj, char* name){
	struct nlist *np;
	for (np = table_obj->table[hash(name)]; np != NULL; np = np->next){
		if (strcmp(name, np->name) == 0)
		return np; /* found */
	}
	return NULL; /* not found */
}


/* insert: put (name, count) in hashtab */
/* This insert returns a nlist node. Thus when you call insert in your main function */
/* you will save the returned nlist node in a variable (mynode).*/
/* Then you can set the starttime and finishtime from your main function: */
/* mynode->starttime = starttime; mynode->finishtime = finishtime; */
struct nlist *insert(struct threaded_hash_table* table_obj, char* name, int count){
	struct nlist *np;
	unsigned hashval;
	//There are 2 cases:
	if ((np = lookup(table_obj, name)) == NULL) { 
		/* case 1: the pid is not found, so you have to create it with malloc.
		 * Then you want to set the pid, command and index */
		//np = (struct nlist *) malloc(sizeof(*np));
		np = new_nlist(name);
		if (np == NULL){
			free(name);
			return NULL;
		}
		
		np->count = 1;
		hashval = hash(name);
		np->next = table_obj->table[hashval];
		table_obj->table[hashval] = np;
	} else {
		//inset on the same name means what?? probably better to just increment the existing name
		np->count += 1;
	}  
	return np;
}

void print_hash_table(struct threaded_hash_table* hash_obj){
	for(int i = 0; i < HASH_SIZE; i++){
		printf("nodes at bucket %d:\n", i);
		print_nlist(hash_obj->table[i]);
	}
}

void free_hash_table(struct thraeded_hash_table* table_obj){
	for(int i = 0; i < HASH_SIZE; i++){
		if(table_obj->table[i] != NULL){
			free_nlist_recursive(table_obj->table[i]);
		}
	}
	free(table_obj->table);
	free(table_obj);
}

int get_count(struct threaded_hash_table* table_obj, char* name){







