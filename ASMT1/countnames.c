#include <stdio.h>
#include <stdlib.h>

int main(){

	FILE* my_file  = fopen("names.txt", "r");
	if(my_file == NULL){
		printf("cannot open file\n");
	}

	int name_length = 30;
	int num_names = 100;
	char names[num_names][name_length];

	//initialize the strings in names to zero
	for(int i = 0; i < num_names; i++){
		for(int j = 0; j < name_length; j++){
			names[i][j] = 0;
		}
	}

	char line[name_length];
	while(fgets(line, name_length, my_file) != NULL){
		printf("%s", line);
	}

	int status = fclose(my_file);

	return 0;
}
