#include <stdio.h>
#include <string.h>

struct ContactNode_struct {
	char* conactName;
	char* contactPhoneNumber;
	struct ContactNode_struct* nextNodePtr;
};

typedef struct ContactNode_struct ContactNode;

void InitializeContactNode(ContactNode* node, char* name, char* phone_num){
	node->contactName = name;
	node->contactPhoneNumber = phone_num;
}


int main(void) {
	ContactNode* temp = (ContactNode*) malloc(sizeof(ContactNode));
	
	
	
	return 0;
}

