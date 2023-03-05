#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**
 * Description: This module counts the number of each unique name in a file and then
 * 		outputs the names and their counts with one name and count per line
 * Author names: Andrew Townsend
 * Author emails: andrew.townsend@sjsu.edu
 * Last modified date: 2/19/2023
 * Creation date: 2/19/2023
 **/


int name_length = 30;
int num_names = 100;


/**
 * This function searches for the location of a name in a list of names
 * Assumption: the length of the name string is less than or equal to the name_length,
 * 		the size of the names array is in accordance with the global variables
 * 		name length and num names, those global variables are available,
 * 		the number of names is positive
 * Input parameters: names (a two D array of charecters), name(a string), num_names(an int) 
 * Returns: an integer, with -1 meaining not found and anything else meaning the location of the
 * 		found name in the array
**/
int find_name(char names[num_names][name_length], char* name, int num_names){
        for(int i = 0; i < num_names; i++){
                if(strcmp(names[i], name) == 0){
        		return(i);
		}
	}
        return -1;
}


/**
 * This function reads a file and records the occurance of each unique name as well as the number of
 * times that each unique name is mentioned. Then it outputs those values in a list
 * Assumption: this program has read and write privaleges
 * Input parameters: the location of the file to be read
 * Returns: an integer corresponding to the status of the program output
**/
int main(int argc, char* argv[]){
	if(argc == 1){
		exit(0);
	}

	FILE* my_file  = fopen(argv[1], "r");
	if(my_file == NULL){
		printf("cannot open file\n");
	}

	char names[num_names][name_length];
	int names_counter[num_names];
	int num_stored_names = 0;

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
		//if(strcmp(line, "\n") == 0){
		if(line[0] == '\n' || line[0] = ' '){
			fprintf(stderr, "Warning - line %d is empty.\n", line_counter); 
		} else {
			// remove the newline at the end of the most recently read line with a null
			if(line[strlen(line) - 1] == '\n'){
				line[strlen(line) - 1] = '\0';
			}
			//look to see if the name in this line is stored in the names array
			int location = find_name(names, line, num_names);
			if(location != -1){
				//printf("found at location %d", location);
				names_counter[location] = names_counter[location] + 1;
			}
			//if so, increment the number of instances of that name in the counter array
			//else, add the recently read name to the names array and then set the counter
			//for the recently added name to one and increment the number of stored names
			else{
				strcpy(names[num_stored_names], line);
				//names[num_stored_names] = line;
				names_counter[num_stored_names] = 1;
				num_stored_names++;
			}
			//printf("%s", line);
		}
		line_counter++;
	}
	fclose(my_file);

	for(int i = 0; i < num_stored_names; i++){
		printf("%s: %d\n", names[i], names_counter[i]);
	}

	return 0;
}
