#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <fcntl.h>


int read_to_pipe(int my_pipe[], char* file_name){
	// Close the write end of the pipe
	close(my_pipe[0]);
	// Read data from the input file and write it to the pipe
        int name_length = 30;
	char line[name_length];
	FILE* my_file  = fopen(file_name, "r");
	
	if(my_file == NULL){
		fprintf(stderr, "range: cannot open file\n");
	}
	
	while(fgets(line, name_length, my_file) != NULL){
		size_t len = strlen(line);
		if(write(my_pipe[1], line, len) != len){
			fprintf(stderr, "error writing to pipe\n");
                        return 1;
		}
	}
	// Close the input file and the write end of the pipe
	fclose(my_file);
	close(my_pipe[1]);
	return 0;
}


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

			/*	
			printf("==========");
			printf("child process #%d, actual id: %d\n", i, getpid());
			printf("reading file: %s\n", argv[i]);
			printf("i = %d\n", i);
			printf("==========\n");

			// Close the write end of the pipe
            		close(fd[0]);

			// Read data from the input file and write it to the pipe
			int name_length = 30;
			char line[name_length];
			//int bytes_read;
			FILE* my_file  = fopen(argv[i], "r");

			if(my_file == NULL){
				fprintf(stderr, "range: cannot open file\n");
			}

			while(fgets(line, name_length, my_file) != NULL){
				size_t len = strlen(line);
				if(write(fd[1], line, len) != len){
					fprintf(stderr, "error writing to pipe\n");
					exit(1);
				}
			}

			// Close the input file and the write end of the pipe
			fclose(my_file);
			close(fd[1]);
			exit(0);*/
		}
	}
	printf("parent process, pid: %d, actual id: %d\n", pid, getpid() );
	close(fd[1]);
	
	char buff[30];
	int bytes_read;
	while ((bytes_read = read(fd[0], buff, sizeof(buff))) > 0) {
		printf("%.*s", bytes_read, buff);
    	}

	// Close the read end of the pipe
	close(fd[0]);

	// Wait for all child processes to complete
	for (int i = 1; i <= argc; i++) {
        	wait(NULL);
	}

	printf("All child processes have completed\n");
	return 0;
}
