#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc == 1){
		exit(0);
	}

	FILE* my_file  = fopen(argv[1], "r");
	if(my_file == NULL){
		printf("cannot open file\n");
	}

	int name_length = 30;
	int num_names = 100;
	char names[num_names][name_length];
	int names_counter[num_names];

	//initialize the strings in names to zero and the counter to zero
	for(int i = 0; i < num_names; i++){
		names_counter[i] = 0;
		for(int j = 0; j < name_length; j++){
			names[i][j] = '\0';
		}
	}

	char line[name_length];
	int line_counter = 1;
	// start reading the file line by line
	while(fgets(line, name_length, my_file) != NULL){
		//check to see if an empty line
		//if so, ignore the line and print to standard error "Warning - line _ is empty.\n"
		//printf("printing the ascii values of line %d: ", line_counter);
		//int i = 0;
		//while(line[i] != '\0'){
		//	printf("%d, ", line[i]);
		//	i++;
		//}
		//printf("\n");
		if(strcmp(line, "\n") == 0){
			fprintf(stderr, "Warning - line %d is empty.\n", line_counter); 
		} else {
			printf("%s", line);
		}
		line_counter++;
	}

	int status = fclose(my_file);

	return 0;
}
