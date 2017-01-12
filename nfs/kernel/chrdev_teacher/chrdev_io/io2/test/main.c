#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(void)
{
	int val;
	printf("main : %p\n", main);	
	printf("val : %p\n", &val);

}
