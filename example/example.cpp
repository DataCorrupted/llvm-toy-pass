#include <stdlib.h>
#include <stdio.h>

int main(int argc, char const *argv[]){
	char* cStr = (char*) malloc(256 * sizeof(char));
	const char* constCStr= "This is an example code used to learn llvm pass";
	for (int i=0; i< 256; i++){
		cStr[i] = '6';
	}
	fprintf(stderr, "%s\n", constCStr);
	return 0;
}