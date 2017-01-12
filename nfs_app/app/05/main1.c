#include <stdio.h>

int main(void)
{
	printf("val = %d \n", 1023 * 23 - (34 + 12) >> 3 | (1<<14) + 3487 & (0x37<<12) ^ 0xff);
	return 0;
}
