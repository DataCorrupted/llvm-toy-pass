#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>


unsigned get_unsigned_int(){
	int c = '0';
	unsigned input = 0;
	while ((c >= '0') && (c <= '9')) {
		// '0' - '0' is an unsigned overflow.
		// it turns into 48 - 48
		// further into 48 + (unsigned)(-48)
		// and it results a wrap(to 0).
		input = input * 10 + (c - '0');
		c = getchar();
	}
	return input;
}

int main(int argc, char** argv){

	printf("int_overflow.c\n");
	int a = (int) get_unsigned_int();
	int u = (unsigned) a;

	printf("Got input a == %d\n", a);
	printf("Unsigned version %d\n", u);
	
	int mul = a * 10; 					// a > 214748364 leads to crash
	printf("mul0 %d\n", mul);

		mul = a * (-100); 				// a > 21474836 leads to crash
		printf("mul1 %d\n", mul);

	int add = a + 2147400000;			// a > 83647 leads to crash
	printf("add0 %d\n", add);

	int shl = a << 20;					// a > 2047 would crash
	printf("shl0 %d\n", shl);

		shl = 1 << a;					// a >= 32 would crash
		printf("shl1 %d\n", shl);

	int div = 10 / (a - 3);				// a == 3 crash
	printf("div0 %d\n", div);
	
		div = a / (div - 1);			// a == 13 +- 5 would crash
		printf("div1 %d\n", div);
	
		div = a / (a - 10);				// a == 10 crash
		printf("div2 %d\n", div);
	
	unsigned udiv = u / (unsigned) 10;
	printf("div3 %d\n", udiv);

	int rem = a % 12; 					// Won't crash
	printf("rem0 %d\n", rem);
	
		rem = 2147483647 % (a - 30); 	// a == 30 crash.
		printf("rem1 %d\n", rem);
	
	unsigned urem = u % (unsigned) 13;
	printf("rem2 %d\n", urem);

	int sub = a - 2147480000;			// Won't crash
	printf("sub0 %d\n", sub);

//	unsigned usub = ((unsigned) a) - 5;	// a < 5 crash
//	printf("sub1 %d\n", usub);


}