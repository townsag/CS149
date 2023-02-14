#include <stdio.h>

int main(void) {

   /* Type your code here. */
   int numerator;
   int denominator;
   int quotient;
   scanf("%d %d", &numerator, &denominator);
   quotient = numerator/ denominator;
   printf("%d ", quotient);
   quotient = quotient / denominator;
   printf("%d ", quotient);
   quotient = quotient / denominator;
   printf("%d\n", quotient);

   return 0;
}
