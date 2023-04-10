#ifndef ITEM_TO_PURCHASE_H
#define ITEM_TO_PURCHASE_H

#include <string.h>

struct ItemToPurchase {
	char itemName[30];
	int itemPrice;
	int itemQuantity;
};

void MakeItemBlank(struct ItemToPurchase* item);
void PrintItemCost(struct ItemToPurchase* item);
void ReadInItem(struct ItemToPurchase* item);

#endif
