#ifndef ITEM_TO_PURCHASE_H
#define ITEM_TO_PURCHASE_H

#include <string.h>

struct ItemToPurchase {
	char itemName[30];
	int itemPrice;
	int itemQuantity;
}

void MakeItemBlank(struct ItempToPurchase* item){
	strcpy(item->itemName, "none");
	item->price = 0;
	item->itemQuantity = 0;
}

void PrintItemCost(struct ItemToPurchase* item){
	printf("%s %d @ $%d = $%d\n", item->name, item->itemQuantity, item->itemPrice, \
			item->itemQuantity * item->itemPrice);
}

#endif
