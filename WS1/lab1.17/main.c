#include <stdio.h>

int main(void) {

   /* Type your code here. */
   int a; int b;
   scanf("%d %d", &a, &b);
   printf("    %d\n", b);
   printf("    %d%d\n", b, b);
   printf("%d%d%d%d%d%d%d\n", a, a, a, a, b, b, b);
   printf("%d%d%d%d%d%d%d%d\n", a, a, a, a, b, b, b, b);
   printf("%d%d%d%d%d%d%d\n", a, a, a, a, b, b, b);
   printf("    %d%d\n", b, b);
   printf("    %d\n", b);

   return 0;
}
