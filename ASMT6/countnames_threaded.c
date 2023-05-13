#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include "threaded_hash.h"

/*****************************************
//CS149 SP23
//Template for assignment 6
//San Jose State University
//originally prepared by Bill Andreopoulos
*****************************************/


//thread mutex lock for access to the log index
//TODO you need to use this mutexlock for mutual exclusion
//when you print log messages from each thread
pthread_mutex_t tlock1 = PTHREAD_MUTEX_INITIALIZER;


//thread mutex lock for critical sections of allocating THREADDATA
//TODO you need to use this mutexlock for mutual exclusion
pthread_mutex_t tlock2 = PTHREAD_MUTEX_INITIALIZER;

//used to make sure that both threads have finished executing
pthread_cond_t done_cond = PTHREAD_COND_INITIALIZER;
int done = 0;

//thread mutex lock for access to the name counts data structure
//TODO you need to use this mutexlock for mutual exclusion
//pthread_mutex_t tlock3 = PTHREAD_MUTEX_INITIALIZER;
//encapsulated in hash ^^

void* thread_runner(void*);
pthread_t tid1, tid2;


//struct points to the thread that created the object.
//This is useful for you to know which is thread1. Later thread1 will also deallocate.
struct THREADDATA_STRUCT
{
pthread_t creator;
};
typedef struct THREADDATA_STRUCT THREADDATA;

THREADDATA* p=NULL;


//variable for indexing of messages by the logging function.
int logindex=1;
int *logip = &logindex;

typedef struct __myarg_t{
        char* file_name;
} myarg_t;

typedef struct __myret_t{
        int n;
} myret_t;


//for holding name counts
threaded_hash_table_t* hash_obj;


/*********************************************************
// function main
*********************************************************/
int main(int argc, char* argv[]){
	// TODO similar interface as A2: give as command-line arguments three filenames of 
	// numbers (the numbers in the files are newline-separated).
	
	if(argc < 3){ printf("too few arguments were entered\nplease enter two files to be counted\n"); exit(1);}
	if(argc > 3){ printf("too many arguments were entered\nplease enter two files to be counted\n"); exit(3);}

	hash_obj = new_threaded_hash_table();

	myarg_t arg1;
	arg1.file_name = argv[1];
	//printf("file one is called <%s>\n", argv[1]);
	myarg_t arg2;
	arg2.file_name = argv[2];
	//printf("file two is called <%s>\n", argv[2]);
	fflush(stdout);

	printf("create first thread\n");
	pthread_create(&tid1,NULL,thread_runner,&arg1);
	
	printf("create second thread\n");
	pthread_create(&tid2,NULL,thread_runner,&arg2);
	
	printf("wait for first thread to exit\n");
	pthread_join(tid1,NULL);
	printf("first thread exited\n");	
	
	printf("wait for second thread to exit\n");
	pthread_join(tid2,NULL);
	printf("second thread exited\n");
	//TODO print out the sum variable with the sum of all the numbers
	
	//print_hash_table(hash_obj);
	printf("\n\n==========counts==========\n");
	pretty_print(hash_obj);
	free_hash_table(hash_obj);
	exit(0);
}//end main


/**********************************************************************
// function thread_runner runs inside each thread
**********************************************************************/
void* thread_runner(void* x){
	pthread_t me;

	myarg_t* args = (myarg_t*)x;

	me = pthread_self();
	//printf("This is thread %ld (p=%p)\n",me,p);

	pthread_mutex_lock(&tlock2); // critical section starts
	if (p==NULL) {
		p = (THREADDATA*) malloc(sizeof(THREADDATA));
		p->creator=me;
	}
	pthread_mutex_unlock(&tlock2); // critical section ends

	pthread_mutex_lock(&tlock1);
	if (p!=NULL && p->creator==me) {
		printf("log index %d: This is thread %ld and I created THREADDATA %p\n", logindex++, me, p);
	} else {
		printf("log index %d: This is thread %ld and I can access the THREADDATA %p\n",logindex++,me,p);
	}
	pthread_mutex_unlock(&tlock1);
	

	FILE* my_file = fopen(args->file_name, "r");

        if(my_file == NULL){
                fprintf(stderr, "cannot open file %s\n", args->file_name);
		exit(1);
	}

	pthread_mutex_lock(&tlock1);
	printf("log index %d: this is thread %ld and I opened the file %s\n", logindex++, me, args->file_name);
	pthread_mutex_unlock(&tlock1);

	char *line = NULL;
	size_t bufsize = 0;
	ssize_t nread;
	int count = 0;
	
	while ((nread = getline(&line, &bufsize, my_file)) != -1) {
		count++;
		if(line[strlen(line) - 1] == '\n'){
			line[strlen(line) - 1] = '\0';
		}
		if(strcmp(line, "\n") == 0 || strcmp(line, " \n") == 0 || strcmp(line, "") == 0){
                        fprintf(stderr, "Warning - file %s line %d is empty.\n", args->file_name, count);
                } else {
			char* temp = strdup(line);
			add_name(hash_obj, temp);
			//free(temp);
			//printf("read the line: %s\n", line);
		}
	}
	free(line);

    	fclose(my_file);

	// critical section starts
	pthread_mutex_lock(&tlock2);
	if (p!=NULL && p->creator==me) {
		pthread_mutex_lock(&tlock1);
		printf("log index %d: This is thread %ld and I delete THREADDATA\n", logindex++, me);
		pthread_mutex_unlock(&tlock1);
		
		while(done == 0){
			pthread_cond_wait(&done_cond, &tlock2);
		}
		free(p);
	} else {
		done = 1;
		pthread_cond_signal(&done_cond);
		pthread_mutex_lock(&tlock1);
		printf("log index %d: This is thread %ld and I can access the THREADDATA\n", logindex++, me);
		pthread_mutex_unlock(&tlock1);
	}
	pthread_mutex_unlock(&tlock2);
	//critical section ends
	
	pthread_exit(NULL);
	//return NULL;
}
//end thread_runner
