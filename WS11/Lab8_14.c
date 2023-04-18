#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct ContactNode_struct {
	char* contactName;
	char* contactPhoneNumber;
	struct ContactNode_struct* nextNodePtr;
};

typedef struct ContactNode_struct ContactNode;

void InitializeContactNode(ContactNode* node, char* name, char* phone_num){
	node->contactName = name;
	node->contactPhoneNumber = phone_num;
	node->nextNodePtr = NULL;
}

char* GetName(ContactNode* node){
	return node->contactName;
}

char* GetPhoneNumber(ContactNode* node){
	return node->contactPhoneNumber;
}

void InsertAfter(ContactNode* tail, ContactNode* next){
	tail->nextNodePtr = next;
}

ContactNode* GetNext(ContactNode* node){
	return node->nextNodePtr;
}

void PrintContactNode(ContactNode* node){
	printf("Name: %s\n", GetName(node));
	printf("Phone number: %s\n\n", GetPhoneNumber(node)); 
}

void PrintList(ContactNode* head){
	PrintContactNode(head);
	if(GetNext(head) != NULL){
		PrintList(GetNext(head));
	}
}

int main(void) {
	ContactNode* head = (ContactNode*) malloc(sizeof(ContactNode));
	ContactNode* tail = head;

	char nameBuffer[30];
	char phoneBuffer[30];
	int count = 1;
	while(fgets(nameBuffer, 30, stdin) != NULL){
		fgets(phoneBuffer, 30, stdin);
		ContactNode* temp = (ContactNode*) malloc(sizeof(ContactNode));
		nameBuffer[strlen(nameBuffer) - 1] = '\0';
		phoneBuffer[strlen(phoneBuffer) - 1] = '\0';
		char* tempName = (char*) calloc(strlen(nameBuffer), sizeof(char));
		char* tempPhone = (char*) calloc(strlen(phoneBuffer), sizeof(char));
		strcpy(tempName, nameBuffer);
		strcpy(tempPhone, phoneBuffer);


		printf("Person %d: %s, %s\n", count++, tempName, tempPhone);

		InitializeContactNode(temp, tempName, tempPhone);
		InsertAfter(tail, temp);
		tail = temp;
	}
	
	//PrintContactNode(tail);

	printf("\nCONTACT LIST\n");
	ContactNode* tempHead = head;
	head = GetNext(head);

	free(tempHead);
	
	PrintList(head);
		
	return 0;
}

