#include <stdio.h>

int main(int argc, char** argv){
	char b = 0;
	b = getchar();
	int a = 10 / ((int) b - 48);
	fprintf(stderr, "%d\n", a);

}