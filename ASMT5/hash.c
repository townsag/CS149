/**
* The reason for using a hashtable is to store the commands in hash slots by their pids.
* This way you can lookup a command by a pid and retrieve all the info you need:
* command, index (this index means the line number in the text file you read),
* start time, and anything else you might need.
*
* A hashtable (as you might remember from CS146)
* has slots and each slot contains a linked list of nodes
* (these are the entries that contain all the command info).
* Thus the hashtable (see hashtab array variable below) is
* implemented as an array of nlists. Each array position is a
* slot and a linked list of nlist nodes starts at each array slot.
* Each array position is a hahstable slot.
*
* You find the hashtable slot for a pid by using a hash function,
* which will map the pid to a hashtable slot (array position).
*
* You can copy this entire code directly in your .c code. No need to have
* many files.
*
*
* This nlist is a node, which stores one command's info in the hashtable.
*
* The char *name and char *defn you can remove.
* The nlist *next field is a pointer to the next node in the linked list.
* There is one hashtable slot in each array position,
* consequently there is one linked list of nlists under a hashtable slot.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "hash.h"

struct nlist* new_nlist(pid_t pid, char* command, int index){
	struct nlist* temp = (struct nlist*)calloc(1, sizeof(struct nlist));
	if(temp == NULL){
		printf("failed to allocate new nlist\n");
		return NULL;
	}
	temp->command = command;
	temp->pid = pid;
	temp->index = index;
	return temp;
}

void set_start(struct nlist* to_assign, struct timespec start){
	to_assign->start = start;
}

void add_start(struct nlist* to_start){
	clock_gettime(CLOCK_MONOTONIC, &(to_start->start));
}

void add_stop(struct nlist* to_stop){
	clock_gettime(CLOCK_MONOTONIC, &(to_stop->stop));
}

double get_duration(struct nlist* node){
	return (node->stop.tv_sec - node->start.tv_sec);
}

void print_node(struct nlist* node){
	printf("Command: %s, index: %d, pid: %d\n", node->command, node->index, node->pid);
}

void print_nlist(struct nlist* head){
	if(head != NULL){
		print_node(head);
		print_nlist(head->next);
	}
}

void free_nlist(struct nlist* to_free){
	free(to_free->command);
	free(to_free);
}

void free_nlist_recursive(struct nlist* to_free){
	free(to_free->command);
	if(to_free->next != NULL){
		free_nlist_recursive(to_free->next);
	}
	free(to_free);
}

struct hash_table* new_hash_table(){
	struct hash_table* temp = (struct hash_table*) calloc(1, sizeof(struct hash_table));
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
unsigned hash(int pid){
	return pid % HASH_SIZE;
}

/* lookup: look for pid in hashtab */
/* This is traversing the linked list under a slot of the hash
table. The array position to look in is returned by the hash
function */
struct nlist *lookup(struct hash_table* table_obj, int pid){
	struct nlist *np;
	for (np = table_obj->table[hash(pid)]; np != NULL; np = np->next){
		if (pid == np->pid)
		return np; /* found */
	}
	return NULL; /* not found */
}


/* insert: put (name, defn) in hashtab */
/* This insert returns a nlist node. Thus when you call insert in your main function */
/* you will save the returned nlist node in a variable (mynode).*/
/* Then you can set the starttime and finishtime from your main function: */
/* mynode->starttime = starttime; mynode->finishtime = finishtime; */
struct nlist *insert(struct hash_table* table_obj, int pid, char* command, int index){
	struct nlist *np;
	unsigned hashval;
	//There are 2 cases:
	if ((np = lookup(table_obj, pid)) == NULL) { 
		/* case 1: the pid is not found, so you have to create it with malloc.
		 * Then you want to set the pid, command and index */
		//np = (struct nlist *) malloc(sizeof(*np));
		np = new_nlist(pid, command, index);
		if (np == NULL){
			free(command);
			return NULL;
		}

		hashval = hash(pid);
		np->next = table_obj->table[hashval];
		table_obj->table[hashval] = np;
	} else {
		//inset on the same PID means that the process using that pid originally 
		//has already finished and I need to replace that node with a new node for
		//that process
		//could also just update the node to hold the new values for command and index
		free(np->command);
		np->command = command;
		np->index = index;
	} 
	/* case 2: the pid is already there in the
	hashslot, i.e. lookup found the pid. In this case you can either
	do nothing, or you may want to set again the command and index
	(depends on your implementation). */
	//free((void *) np->defn); free previous defn 
	return np;
}

void print_hash_table(struct hash_table* hash_obj){
	for(int i = 0; i < HASH_SIZE; i++){
		printf("nodes at bucket %d:\n", i);
		print_nlist(hash_obj->table[i]);
	}
}

void free_hash_table(struct hash_table* table_obj){
	for(int i = 0; i < HASH_SIZE; i++){
		if(table_obj->table[i] != NULL){
			free_nlist_recursive(table_obj->table[i]);
		}
	}
	free(table_obj->table);
	free(table_obj);
}




