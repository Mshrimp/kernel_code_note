#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>

int main(void)
{
	int val;
	printf("main : %p\n", main);	
	printf("val : %p\n", &val);

	int ret;
	char buf[100] = {};
	ret = read(0, buf, 100);
	printf("ret = %d\n", ret);
}
