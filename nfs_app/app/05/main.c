#include <stdio.h>

int main(void)
{
	register int a = 10;
	if(a > 10)
	{
		printf("a > 10\n");
	} 
	else if(a < 10)
	{
		printf("a < 10\n");
	} 
	else
	{
		printf("a == 10\n");
	}
	return 0;
}
