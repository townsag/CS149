#include <stdio.h>

int main(void) {
   double x;
   double y;
   double z;
   
   /* Type your code here. Note: Include the math library above first. */
   scanf("%lf %lf %lf", &x, &y, &z);
   //printf("%lf %lf %lf\n", x, y, z);
   double a1 = pow(x, z);
   double a2 = pow(x, pow(y, 2));
   double a3 = fabs(y);
   double a4 = sqrt(pow(x*y, z));

   printf("%0.2lf ", a1);
   printf("%0.2lf ", a2);
   printf("%0.2lf ", a3);
   printf("%0.2lf\n", a4);

   return 0;
}
