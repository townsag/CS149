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

int read_to_pipe(int my_pipe[], char* file_name, sem_t sem);
void initialize(struct my_data array[], int len);
int find_name(struct my_data array[], char* name, int num_names);


int main(int argc, char* argv[]){
	int fd[2];
	//sem_t* sem;
	sem_t sem;
	pid_t pid;

	if(pipe(fd) == -1){
		fprintf(stderr, "pipe failed\n");
		exit(1);
	}

	//sem = sem_open("/my_semaphore", O_CREAT, 0666, 1);
	//if (sem == SEM_FAILED) {
	//	fprintf(stderr, "Error: Failed to create semaphore\n");
	//	exit(1);
	//
	//}
	if (sem_init(&sem, 1, 1) != 0) {
        	perror("Error initializing semaphore");
        	exit(-1);
    	}
	
	//printf("argc: %d\n", argc);
	for(int i = 1; i < argc; i++){
		pid = fork();
		if(pid < 0){
			fprintf(stderr, "fork failed\n");
		}
		else if(pid == 0){
			exit(read_to_pipe(fd, argv[i], sem));
		}
	}

	//printf("parent process, pid: %d, actual id: %d\n", pid, getpid() );
	close(fd[1]);
	
	struct my_data namecounts[MAX_NAMES];
	initialize(namecounts, MAX_NAMES);
	int num_names = 0;

	//read a line from the buffer
	char buff[NAME_LENGTH];
	size_t num;
	//int status;
	int num_dead = 0;
	//while((num = read(fd[0], buff, NAME_LENGTH)) > 0){
	while((num = read(fd[0], buff, NAME_LENGTH)) > 0 || num_dead < (argc - 1)){
	//while((num = read(fd[0], buff, NAME_LENGTH)) > 0 && (pid = waitpid(-1, &status, WNOHANG)) >= 0){
		//if the name is in the array of my_data structs then increment the counter
		
		if(num != 0){

			//printf("this is num: %ld\n", num);
			if(buff[strlen(buff) - 1] == '\n'){
				buff[strlen(buff) - 1] = '\0';
                	}
			int name_index = find_name(namecounts, buff, num_names);
			if(name_index != -1){
				namecounts[name_index].count = namecounts[name_index].count + 1;
			} else {
				strcpy(namecounts[num_names].name, buff);
				namecounts[num_names].count = 1;
				num_names++;
			}
		}

		//printf("got here\n");
		//fflush(stdout);
		
		if(waitpid(-1, NULL, WNOHANG) > 0){
			num_dead++;
			//printf("===================num dead: %d\n", num_dead);
		}
	}

	if(num == -1){
		fprintf(stderr,"pipe read error");
	}	
	

	// Close the read end of the pipe
	close(fd[0]);

	
	// Wait for all child processes to complete
	/*
	for (int i = 1; i < argc; i++) {
        	wait(NULL);
	}*/
	/*
	int status;
	while (1) {
        	pid_t child_pid = waitpid(-1, &status, WNOHANG);
        	if (child_pid == 0) {
			// no child process has terminated
            		break;
        	} else if (child_pid > 0) {
            		// child process has terminated
            		printf("Child process %d terminated\n", child_pid);
		}
	}*/

	//printf("All child processes have completed\n");
	//sem_unlink("/my_semaphore");
        //sem_close(sem);
	sem_destroy(&sem);

	for(int i = 0; i < num_names; i++){
		//printf("==========index: %d==========\n", i);
		printf("%s: %d \n", namecounts[i].name, namecounts[i].count);
	}
	
	
	return 0;
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
                        //size_t len = strlen(line);
			//printf("writing to pipe: %s\n", line);
			sem_wait(&sem);
			//printf("writing to pipe: '%s'\n", line);
                        //if(write(my_pipe[1], line, len) != len){
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
