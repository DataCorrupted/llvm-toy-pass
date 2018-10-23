#include <stdio.h>

void print(){
	fprintf(stderr, "Yeah!\n");
}

int main(){
	int a = 0;
	scanf("%d", &a);
	a ++;
	fprintf(stderr, "%d\n", a);
	
	a *= 3;
	fprintf(stderr, "%d\n", a);
	return 0;
}