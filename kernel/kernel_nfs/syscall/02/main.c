#include <stdio.h>
#include <errno.h>
#include <unistd.h>

int my_write(int fd, const char *buf, int size)
{
	int ret;
	asm __volatile__(
		"mov r0, %1\n\t"
		"mov r1, %2\n\t"
		"mov r2, %3\n\t"
		"swi #(0x900000+4)\n\t"
		"mov %0, r0\n\t"
		: "=&r" (ret)
		: "r" (fd), "r" (buf), "r" (size)
		: "r0", "r1", "r2"
	);

	if(ret < 0)
	{
		errno = -ret;
		ret = -1;
	}
	return ret;
}

int main(void)
{
	int ret;
	ret = my_write(3, "hello world\n", 12);	//libc
	if(-1 == ret)
	{
		perror("my_write");
		return -1;
	}
	printf("ret = %d\n", ret);
	return 0;
}


