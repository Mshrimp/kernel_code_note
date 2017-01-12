#include <stdio.h>
#include <unistd.h>

int main(void)
{
	int ret;
	ret = write(1, "hello world\n", 12);	//libc
	if(-1 == ret)
	{
		perror("write");
		return -1;
	}
	printf("ret = %d\n", ret);
	
	return 0;
}


