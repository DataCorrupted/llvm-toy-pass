#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void passPtr(void* voidPtr){
	int* intPtr = (int*) voidPtr;
	*(intPtr + 1) = 100;
}

int main(int argc, char const *argv[]){
	int intArray[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	int uIntArray[argc * 3 + (int) argv[0]];
	uIntArray[0] = 1;

	void * intPtr = malloc(argc * sizeof(int));

	passPtr(intPtr);
	passPtr(intPtr + 5);

	for (int i=0; i<10; i++){
		fprintf(stderr, "%d ", *((int*)	intPtr + i));
	}
	fprintf(stderr, "\n");

	const char* msgStr = "This is a message.";
	fprintf(stderr, "%s\n", msgStr);

	char* otherStr = (char*) malloc(128 * sizeof(char));
	
	return 0;
}
