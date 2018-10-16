#include <stdlib.h>
#include <stdio.h>

int inc(int* i){ 
	(*i)++; 
	return *i;
}

int dec(int* i){
	(*i)--;
	return *i;
}

int main(int argc, char const *argv[]){
	char* cStr = (char*) malloc(256 * sizeof(char));
	const char* constCStr= "This is an example code used to learn llvm pass";
	for (int i=0; i< 256; i++){
		cStr[i] = '6';
	}

	fprintf(stderr, "%s\n", constCStr);

	fprintf(stderr, "%s\n", "Testing inc function.");
	for (int i=0; i<10; inc(&i)){
		fprintf(stderr, "%d ", i);
	}
	fprintf(stderr, "\n");
	return 0;
}
