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
* TODO: You will need to adopt this code a little.
*
* The char *name and char *defn you can remove.
* The nlist *next field is a pointer to the next node in the linked list.
* There is one hashtable slot in each array position,
* consequently there is one linked list of nlists under a hashtable slot.
*/

#include <stdio.h>
#include <stdlib.h>
#include "hash.h"


//struct nlist { 
	/* table entry: */
	//struct nlist *next;  next entry in chain 
	//char *name;  defined name, can remove 
	//char *defn;  replacement text, can remove 
	/* starttime */
	/* finishtime */
	/* index // this is the line index in the input text file */
	/* pid // the process id. you can use the pid result of wait to lookup in the hashtable */
	/* char *command; // command. This is good to store for when you decide to restart a command */
//};

//static struct nlist *hashtab[HASHSIZE];  pointer table 

struct nlist* new_nlist(pid_t pid, char* command, int index){
	struct nlist* temp = (nlist*)calloc(1, sizeof(struct nlist));
	if(temp == NULL){
		printf("failed to allocate new nlist\n");
		return NULL;
	}
	temp->command = command;
	temp->pid = pid;
	temp->index = index;
	return temp;
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
	return temp;
}


/* This is the hash function: form hash value for string s */
/* TODO change to: unsigned hash(int pid) */
/* TODO modify to hash by pid . */
/* You can use a simple hash function: pid % HASHSIZE */
unsigned hash(int pid){
	//unsigned hashval;
	/*for (hashval = 0; *s != '\0'; s++){
		hashval = *s + 31 * hashval;
	}*/
	return pid % HASHSIZE;
}

/* lookup: look for s in hashtab */
/* TODO change to lookup by pid: struct nlist *lookup(int pid) */
/* TODO modify to search by pid, you won't need strcmp anymore */
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
/* TODO: change this to insert in hash table the info for a new pid and its command */
/* TODO: change signature to: struct nlist *insert(char *command, int pid, int index). */
/* This insert returns a nlist node. Thus when you call insert in your main function */
/* you will save the returned nlist node in a variable (mynode).*/
/* Then you can set the starttime and finishtime from your main function: */
/* mynode->starttime = starttime; mynode->finishtime = finishtime; */
struct nlist *insert(struct hash_table* table_obj, int pid, char* command, int index){
	struct nlist *np;
	unsigned hashval;
	//TODO change to lookup by pid. There are 2 cases:
	if ((np = lookup(pid)) == NULL) { 
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
	} else { } 
	/* case 2: the pid is already there in the
	hashslot, i.e. lookup found the pid. In this case you can either
	do nothing, or you may want to set again the command and index
	(depends on your implementation). */
	//free((void *) np->defn); free previous defn 
	return np;
}
