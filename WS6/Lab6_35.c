#include <stdio.h>


void swap(int array[], int i, int j){
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
}


int partition(int array[], int low, int high){
        int pivot_value = array[high];
        int i = low - 1;

        for(int j = low; j <= high - 1; j++){
                if(array[j] < pivot_value){
                        i++;
                        swap(array, i, j);
		}
	}
	swap(array, i + 1, high);
	return i + 1;
}



void quick_sort(int array[], int low, int high){
	if(low < high){
		int pivot = partition(array, low, high);
		quick_sort(array, low, pivot - 1);
		quick_sort(array, pivot + 1, high);
	}
}


void SortArray(int sortingList[], int numVals){
        quick_sort(sortingList, 0, numVals - 1);
}


int main(void){
	int num_elements;
	scanf("%d", &num_elements);

	int array[num_elements];
	for(int i = 0; i < num_elements; i++){
		scanf("%d", &array[i]);
	}

	SortArray(array, num_elements);

	for(int i = 0; i < num_elements; i++){
		printf("%d,", array[i]);
	}
	printf("\n");


	return 0;
}
