#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "utils.h"


//This function reads in a line from a file (or stdin) and then returns a pointer to that input
//because this function allocates memory, it is the job of the caller to deallocate that memory
//this function makes no assumtions about the size of the line, only that the input file is reachable
//this function returns a NULL pointer if there is some sort of failure or if the EOF is reached
char* read_and_allocate(FILE* input){

        //printf("entering RAA\n");
        size_t buffer_size = INITIAL_BUFFER_SIZE;
        char* buffer = (char *)malloc(buffer_size*sizeof(char));

        if(buffer == NULL){
                printf("failed to allocate buffer\n");
                return NULL;
        }

        char c;
        size_t index_to_write = 0;

        c = fgetc(input);
        while(c != '\n' && !feof(input)){
                //check to see if the buffer is full
                if(index_to_write == buffer_size - 2){  //leave room for null at end of string
                        //resize the buffer
                        buffer_size *= 2;
                        char* temp = (char*) realloc(buffer, buffer_size * sizeof(char));
                        if(temp == NULL){
                                printf("failed to reallocate memory\n");
                                free(buffer);
                                return NULL;
                        }
                        buffer = temp;
                }
                buffer[index_to_write++] = c;
                c = fgetc(input);
        }
        buffer[index_to_write] = '\0';

        //check for empty buffer and EOF case
        if(index_to_write == 0 && feof(input)){
                printf("reached EOF logic\n");
                free(buffer);
                return NULL;
        }

        //printf("leaving RAA\n");

        return buffer;
}

//this function recursively frees a linked list
//this function makes the assumption that the head of the linked list is not a null pointer
//this function does not return anything
void free_list(struct my_node* head){

        struct my_node * temp = head->next;
        free(head->line);
        free(head);
        if(temp != NULL){
                free_list(temp);
        }
}

//this function prints the contents of a linked list
//this function makes very few assumptions, for example it can also print a null pointer head linked list
//this function does not return anything
void print_list(struct my_node* head){

        struct my_node* temp = head;
        while(temp != NULL){
                printf("Line %d: %s\n", temp->line_index, temp->line);
                temp = temp->next;
        }
}
