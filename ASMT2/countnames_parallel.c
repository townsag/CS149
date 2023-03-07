#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <fcntl.h>
#include <semaphore.h>


#define NAME_LENGTH 30
#define MAX_NAMES 100


struct my_data {
	char name[NAME_LENGTH];
	int count;
};


/**
 * This function reads from a file and then writes each line of the file to a pipe
 * The pipe is locked using a semephore so that each writer to the pipe can be sure that
 * its data is not muddled by another writer
 * Assumption:	this function called from a child process whose parent has both a pipe and a semephore
 * 		the parent process has not yet closed the write end of the pipe and the semephore is 
 * 		able to be requested by at least one of the child processes
 * Input parameters: the pipe, the name of the file to be read, the semephore to be requested
 * Returns: nothing, exits the child process with the code 0 when successful and 1 on file open errors
**/
int read_to_pipe(int my_pipe[], char* file_name, sem_t sem);
/**
 * This function takes an array of structs and initializes all the structs in the array to be zero
 * Assumption: the array has already been allocated, len is greater than zero
 * input parameters: the array to be initialized and the length of the array
 * Returns: nothing
 **/
void initialize(struct my_data array[], int len);
/**
 * This function searches for the name in an array of my_data structs. Uses linear search
 * Assumption: 	the array passed to the function was initialized to zero before data was added, the num_names
 * 		is less than the length of the array
 * Input parameters: 	the array full of name and count pairs starting from the zero index and building
 * 			downward, the name to look for, the number of names in the array that have been added
 * returns: the index of the name in the array (zero indexed) or -1 if the name cant be found
 **/
int find_name(struct my_data array[], char* name, int num_names);
/**
 * This function writes a name that has been read from the buffer into the struct my_data array of names and counts
 * if the name is not already in the array, otherwise it increments the count value for that name
 * Assumtion: the buffer holds a name that was just read, num_names is less than the length of namecounts
 * input parameters: 	the buffer holding the cstring name, the number of names in the namecounts array, the array of
 * 			names
 * returns: the new count of names in the array of namecounts
 **/
int write_name(char buff[], int num_names, struct my_data namecounts[]);

int main(int argc, char* argv[]){
	int fd[2];
	sem_t sem;
	pid_t pid;

	if(pipe(fd) == -1){
		fprintf(stderr, "pipe failed\n");
		exit(1);
	}

	if (sem_init(&sem, 1, 1) != 0) {
        	perror("Error initializing semaphore");
        	exit(-1);
    	}

	for(int i = 1; i < argc; i++){
		pid = fork();
		if(pid < 0){
			fprintf(stderr, "fork failed\n");
		}
		else if(pid == 0){
			exit(read_to_pipe(fd, argv[i], sem));
		}
	}

	//close the write end of the pipe
	close(fd[1]);
	
	struct my_data namecounts[MAX_NAMES];
	initialize(namecounts, MAX_NAMES);
	int num_names = 0;

	//read a line from the pipe into the buffer
	char buff[NAME_LENGTH];
	size_t num;
	int num_dead = 0;

	while((num = read(fd[0], buff, NAME_LENGTH)) > 0 || num_dead < (argc - 1)){
		//check to see if anything was actually read or if the while loops is just waiting for childrent to end
		if(num != 0){
			num_names = write_name(buff, num_names, namecounts);
		}
		//collect the exit code of the dead child process
		if(waitpid(-1, NULL, WNOHANG) > 0){
			num_dead++;
		}
	}

	if(num == -1){
		fprintf(stderr,"pipe read error");
	}	
	

	// Close the read end of the pipe
	close(fd[0]);
	sem_destroy(&sem);

	for(int i = 0; i < num_names; i++){
		printf("%s: %d \n", namecounts[i].name, namecounts[i].count);
	}
	
	return 0;
}


int write_name(char buff[], int num_names, struct my_data namecounts[]){
	if(buff[strlen(buff) - 1] == '\n'){
		buff[strlen(buff) - 1] = '\0';
	}
	int name_index = find_name(namecounts, buff, num_names);
	if(name_index != -1){
		namecounts[name_index].count = namecounts[name_index].count + 1;
		return num_names;
	} else {
		strcpy(namecounts[num_names].name, buff);
		namecounts[num_names].count = 1;
		return num_names + 1;
	}
}

int read_to_pipe(int my_pipe[], char* file_name, sem_t sem){
        // Close the write end of the pipe
        close(my_pipe[0]);
        // Read data from the input file and write it to the pipe
        char line[NAME_LENGTH];
        FILE* my_file  = fopen(file_name, "r");

        if(my_file == NULL){
                fprintf(stderr, "range: cannot open file\n");
		exit(1);
        }

        int line_count = 1;
        while(fgets(line, NAME_LENGTH, my_file) != NULL){
                if(strcmp(line, "\n") == 0 || strcmp(line, " \n") == 0){
                        fprintf(stderr, "Warning - file %s line %d is empty.\n", file_name, line_count);
                } else {
			sem_wait(&sem);
			// instead write the entire 30 charectars so that I can always read 30 charectars
                        if(write(my_pipe[1], line, NAME_LENGTH) != NAME_LENGTH){
				fprintf(stderr, "error writing to pipe\n");
                                return 1;
                        }
			sem_post(&sem);
                }
                line_count++;
        }
        // Close the input file and the write end of the pipe
        fclose(my_file);
        close(my_pipe[1]);
        return 0;
}


void initialize(struct my_data array[], int len){
	for(int i = 0; i < len; i++){
		for(int j = 0; j < NAME_LENGTH; j++){
			array[i].name[j] = '\0';
		}
		array[i].count = 0;
	}
}


int find_name(struct my_data array[], char* name, int num_names){
        for(int i = 0; i < num_names; i++){
                if(strcmp(array[i].name, name) == 0){
        		return(i);
		}
	}
        return -1;
}
