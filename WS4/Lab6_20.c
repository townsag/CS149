#include <stdio.h>


double LapsToMiles(double numLaps){
        return numLaps * 0.25;
}

int main(void){
	double numLaps;
	scanf("%lf", &numLaps);
	double result = LapsToMiles(numLaps);
	printf("%0.2lf\n", result);
	return 0;
}
