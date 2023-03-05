#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <fcntl.h>

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

			printf("==========");
			printf("child process #%d, actual id: %d\n", i, getpid());
			printf("reading file: %s\n", argv[i]);
			printf("i = %d\n", i);
			printf("==========\n");

			int input_fd = open(argv[i], O_RDONLY);
			if(input_fd == -1){
				//change to "range: cannot open file"
				fprintf(stderr,"cannot open file: %s\n", argv[i]);
			}

			// Close the write end of the pipe
            		close(fd[0]);

			// Read data from the input file and write it to the pipe
			char buffer[20];
			int bytes_read;
			while ((bytes_read = read(input_fd, buffer, sizeof(buffer))) > 0) {
				if (write(fd[1], buffer, bytes_read) == -1) {
					fprintf(stderr, "error writing to pipe");
					exit(1);
				}
			}

			// Close the input file and the write end of the pipe
			close(input_fd);
			close(fd[1]);
			exit(0);
		}
	}
	printf("parent process, pid: %d, actual id: %d\n", pid, getpid() );
	close(fd[1]);
	
	char buff[20];
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
