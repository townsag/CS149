#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <fcntl.h>


#define NAME_LENGTH 30
#define MAX_NAMES 100


struct my_data {
	char name[NAME_LENGTH];
	int count;
};

int read_to_pipe(int my_pipe[], char* file_name);
void initialize(struct my_data array[], int len);
int find_name(struct my_data array[], char* name, int num_names);


int main(int argc, char* argv[]){
	int fd[2];
	pid_t pid;

	if(pipe(fd) == -1){
		fprintf(stderr, "pipe failed\n");
		exit(1);
	}
	
	printf("argc: %d\n", argc);
	for(int i = 1; i < argc; i++){
		pid = fork();
		if(pid < 0){
			fprintf(stderr, "fork failed\n");
		}
		else if(pid == 0){
			exit(read_to_pipe(fd, argv[i]));
		}
	}

	printf("parent process, pid: %d, actual id: %d\n", pid, getpid() );
	close(fd[1]);
	
	struct my_data namecounts[MAX_NAMES];
	initialize(namecounts, MAX_NAMES);
	int num_names = 0;

	//read a line from the buffer
	char buff[NAME_LENGTH];
	size_t num;
	while((num = read(fd[0], buff, NAME_LENGTH)) > 0){
		//if the name is in the array of my_data structs then increment the counter
		int name_index = find_name(namecounts, buff, NAME_LENGTH);
		if(name_index != -1){
			namecounts[name_index].count = namecounts[name_index].count + 1;
		} else {
			strcpy(namecounts[num_names].name, buff);
			namecounts[num_names].count = 1;
			num_names++;
		}
	}

	if(num == 0){
		fprintf(stderr,"pipe read error");
	}	
	//if its in the struct array then increment the counter for that name
	//else add it to the struct array with a cound of 1	
	
	/*
	char buff[NAME_LENGTH];
	int bytes_read;
	while ((bytes_read = read(fd[0], buff, sizeof(buff))) > 0) {
		printf("%.*s", bytes_read, buff);
    	}*/

	// Close the read end of the pipe
	close(fd[0]);

	// Wait for all child processes to complete
	for (int i = 1; i <= argc; i++) {
        	wait(NULL);
	}

	printf("All child processes have completed\n");
	return 0;
}


int read_to_pipe(int my_pipe[], char* file_name){
        // Close the write end of the pipe
        close(my_pipe[0]);
        // Read data from the input file and write it to the pipe
        char line[NAME_LENGTH];
        FILE* my_file  = fopen(file_name, "r");

        if(my_file == NULL){
                fprintf(stderr, "range: cannot open file\n");
        }

        int line_count = 1;
        while(fgets(line, NAME_LENGTH, my_file) != NULL){
                if(strcmp(line, "\n") == 0 || strcmp(line, " \n") == 0){
                        fprintf(stderr, "Warning - file %s line %d is empty.\n", file_name, line_count);
                } else {
                        size_t len = strlen(line);
                        if(write(my_pipe[1], line, len) != len){
                                fprintf(stderr, "error writing to pipe\n");
                                return 1;
                        }
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
