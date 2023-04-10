#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ItemToPurchase.h"

int main(void){
	struct ItemToPurchase *item1 = (struct ItemToPurchase*) malloc(sizeof(struct ItemToPurchase));
	struct ItemToPurchase *item2 = (struct ItemToPurchase*) malloc(sizeof(struct ItemToPurchase));

	MakeItemBlank(item1); MakeItemBlank(item2);
	printf("Item 1\n");
	ReadInItem(item1);
	printf("Item 2\n");
	ReadInItem(item2);

	printf("TOTAL COST\n");
	PrintItemCost(item1);
	PrintItemCost(item2);

	printf("Total: $%d\n", (item1->itemPrice * item1->itemQuantity) + (item2->itemPrice * item2->itemQuantity));
	
	
	return 0;
}
