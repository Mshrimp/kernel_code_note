#include <stdio.h>

int my_print(const char *str)
{
	asm __volatile__(
		"mov r0, %0\n\t"
		"swi #(0x900000+378)"
		: 
		: "r" (str)
		: "r0"
	);	
}

int main(void)
{
	my_print("hello main to kernel\n");
	
	return 0;
}

