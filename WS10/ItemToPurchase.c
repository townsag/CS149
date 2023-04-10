#include <string.h>
#include <stdio.h>
#include "ItemToPurchase.h"

void MakeItemBlank(struct ItemToPurchase* item){
	strcpy(item->itemName, "none");
	item->itemPrice = 0;
	item->itemQuantity = 0;
}

void PrintItemCost(struct ItemToPurchase* item){
	printf("%s %d @ $%d = $%d\n", item->itemName, item->itemQuantity, item->itemPrice, item->itemQuantity * item->itemPrice);
}

void ReadInItem(struct ItemToPurchase* item){
	printf("Enter the item name:\n");
	fgets(item->itemName, 30, stdin);

	if(item->itemName[strlen(item->itemName) - 1] == '\n'){
		item->itemName[strlen(item->itemName) - 1] = '\0';
	}

	printf("Enter the item price:\n");
	scanf("%d", &item->itemPrice);
	printf("Enter the item quantity:\n");
	scanf("%d", &item->itemQuantity);
	getchar();
	printf("\n");
}
