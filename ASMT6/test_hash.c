#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "threaded_hash.h"

typedef struct __myarg_t{
	int n;
} myarg_t;

typedef struct __myret_t{
	int n;
} myret_t;

threaded_hash_table_t* hash_obj;

void* mythread(void* arg){
	char* names[] = {"aaaa", "bbbb", "cccc", "dddd", "eeee", "ffff", "gggg", "hhhh", "iiii", "jjjj"};
	myarg_t* m = (myarg_t*) arg;
	for(int i = 0; i < m->n; i++){
		add_name(hash_obj, names[i]);
	}
	myret_t* r = malloc(sizeof(myret_t));
	r->n = m->n;
	return (void*) r;
}

int main(void){
	//int rc;
	hash_obj = new_threaded_hash_table();
	printf("initialized the hash table\n");
	fflush(stdout);

	pthread_t p1;
	pthread_t p2;

	myret_t* ret1;
	myret_t* ret2;

	myarg_t args1;
	myarg_t args2;
	args1.n = 1000;
	args2.n = 2000;

	printf("about to create both threads\n");
	fflush(stdout);

	pthread_create(&p1, NULL, mythread, &args1);
	pthread_create(&p2, NULL, mythread, &args2);

	pthread_join(p1, (void**) &ret1);
	pthread_join(p2, (void**) &ret2);

	printf("1 returned %d\n", ret1->n);
	printf("2 returned %d\n", ret2->n);

	print_hash_table(hash_obj);
	free_hash_table(hash_obj);
	
	free(ret1);
	free(ret2);

	return 0;
}
