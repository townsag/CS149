#include <stdio.h>
#include <string.h>

int main(void) {

        int max_num_words = 20;
        char user_words[max_num_words][30];
        int num_words;
        char has_char;

        scanf("%d", &num_words);
        for(int i = 0; i < num_words; i++){
                scanf("%s", user_words[i]);
        }
        scanf(" %c", &has_char);
	//printf("char:%c,%d\n", has_char, has_char);
        for(int i= 0; i < num_words; i++){
		//printf("looking at word: %s\n", user_words[i]);
                for(int j = 0; j < strlen(user_words[i]); j++){
			//printf("comparing chars:%c,%c\n", has_char, user_words[i][j]);
                        if(user_words[i][j] == has_char){
                                printf("%s,", user_words[i]);
                                break;
                        }
                }
        }
        printf("\n");

        return 0;
}
