#include <stdio.h>

unsigned get_unsigned_int(){
	char c;
	int input = 0;
	while (c = getchar()) {
		if ((c < '0') || (c > '9')){
			break;
		}
		input = input*10 + (c - '0');
	}
	return input;
}

int main(int argc, char** argv){
	unsigned int a = get_unsigned_int();

	int a = 10 / ((int) b - 48);
	fprintf(stderr, "%d\n", a);

}