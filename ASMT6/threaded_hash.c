#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "threaded_hash.h"
#include <string.h>

node_t* new_node(char* name){
	node_t* temp = (node_t*)calloc(1, sizeof(node_t));
	if(temp == NULL){
		printf("failed to allocate new nlist\n");
		return NULL;
	}
	temp->name = name;
	temp->count = 0;
	temp->next = NULL;
	return temp;
}

void print_node(node_t* node){
	printf("Name: %s, count: %d\n", node->name, node->count);
}


list_t* new_list(){
	list_t* temp = (list*)malloc(sizeof(list*));
	if(temp == NULL){
		printf("failed to allocate new list\n");
		return NULL;
	}
	temp->head = NULL;
	pthread_mutex_init(&temp-list_lock, NULL);
}

void print_list(list_t* list){
	pthread_mutex_lock(&list->list_lock);

	node_t* temp = list->head;
	while(temp != NULL){
		print_node(temp);
		temp = temp->next;
	}

	pthread_mutex_unlock(&list->list_lock);
}

void free_list_recursive(list_t* to_free){
	node_t temp = to_free->head;
	node_t look_ahead;
	while(temp != NULL){
		look_ahead = temp->next;
		free(temp->name);
		free(temp);
		temp = look_ahead;
	}
	free(to_free);
}


int insert_list(list_t* list, char* name){
	node_t* temp = (node_t*)malloc(sizeof(node_t));
	if(temp == NULL){
		print("failed to allocate new node for: %s\n", name);
		return -1;
	}
	temp->name = name;
	temp->count = 1;
	
	pthread_mutex_lock(&list->list_lock);
	temp->next = list->head;
	list->head = temp;
	pthread_mutex_unlock(&list->list_lock);
	return 1;
}

int search_list(list_t* list, char* name){
	int count = 0;
	pthread_mutex_lock(&list->list_lock);
	node_t* temp = list->head;
	while(temp != NULL){
		if(strcmp(temp->name, name) == 0){
			count = temp->count;
			break;
		}
		temp = temp->next;
	}
	pthread_mutex_unlock(&list->list_lock);
	return count;
}

int increment_name(list_t* list, char* name){
	node_t* temp = list->head;
	while(temp != NULL){
		if(strcmp(temp->name, name) == 0){
			pthread_mutex_lock(&list->list_lock);
			temp-count += 1;
			pthread_mutex_unlock(&list->list_lock);
			return 1;
		}
		temp = temp->next;
	}
	return -1;
}
			
threaded_hash_table_t* new_threaded_hash_table(){
	threaded_hash_table_t* temp = (threaded_hash_table_t*) calloc(1, sizeof(threaded_hash_table));
	if(temp == NULL){
		printf("failed to allocate hash table struct\n");
		return NULL;
	}
	
	temp->table = (struct list_t**)calloc(HASH_SIZE, sizeof(struct nlist*));
	if(temp->table == NULL){
		printf("failed to allocate hash table table\n");
		free(temp);
		return NULL;
	}
	for(int i = 0; i < HASH_SIZE; i++){
		temp->table[i] = new_list();
		if(temp->table[i] == NULL){
			printf("failed to allocate bucket number: %d\n", i);
			for(int j = 0; j < i; j++){
				free_list_recursive(temp->table[j]);
			}
			return NULL;
		}
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
table. The count of that name is returned or zero */
int lookup_name(threaded_hash_table_t* table_obj, char* name){
	return search_list(table_obj->table[hash(name)], name);
}

int add_name(threaded_hash_table* table_obj, char* name){
	int index = hash(name);
	int result;
	if ((result = increment_name(table_obj->table[index], name)) == -1){
		result = insert_list(table_obj->table[index], name);
	}
	return result;
}

void print_hash_table(threaded_hash_table_t* hash_obj){
	for(int i = 0; i < HASH_SIZE; i++){
		printf("nodes at bucket %d:\n", i);
		print_list(hash_obj->table[i]);
	}
}

void free_hash_table(threaded_hash_table_t* table_obj){
	for(int i = 0; i < HASH_SIZE; i++){
		free_list_recursive(table_obj->table[i]);
	}
	free(table_obj->table);
	free(table_obj);
}


