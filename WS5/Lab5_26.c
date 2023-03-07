#include <stdio.h>
#include <string.h>#include <stdio.h>

int main(void){

        char line[50 + 1];
        fgets(line, 50, stdin);
        int length = strlen(line);
        int count = 0;

        for(int i = 0; i < length; i++){
                if(line[i] != ' ' && line[i] != '.' && line[i] != '!' && line[i] != ','){
                        count++;
                }
        }
        printf("%d\n", count);

        return 0;
}


