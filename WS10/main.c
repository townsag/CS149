#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ItemToPurchase.h"

int main(void){
	struct ItemToPurchase *item1 = (struct ItemToPurchase*) malloc(sizeof(struct ItemToPurchase));
	//struct ItemToPurchase *item2 = (struct ItemToPurchase*) malloc(sizeof(struct ItemToPurchase));

	MakeItemBlank(item1);
	PrintItemCost(item1);
	
	
	return 0;
}
