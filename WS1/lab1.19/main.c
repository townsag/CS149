#include <stdio.h>

int main(void) {
   int userNum;
   int otherUserNum;
   
   printf("Enter integer:\n");
   scanf("%d", &userNum);
   /* Type your code here. */
   printf("You entered: %d\n", userNum);
   printf("%d squared is %d\n", userNum, userNum * userNum);
   printf("And %d cubed is %d!!\n", userNum, userNum *userNum * userNum);
   printf("Enter another integer:\n");
   scanf("%d", &otherUserNum);
   printf("%d + %d is %d\n", userNum, otherUserNum, userNum + otherUserNum);
   printf("%d * %d is %d\n", userNum, otherUserNum, userNum * otherUserNum);
   return 0;
}