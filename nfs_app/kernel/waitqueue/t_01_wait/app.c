#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


void func(int sig)
{
	printf("sig = %d\n", sig);
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		perror("argc");
		return -1;
	}

	if (singal(SIGINT, func) == SIG_ERR)
	{
		perror("singal");
		return -1;
	}

	int fd;

	fd = open(argv[1], O_RDWR);
	if (-1 == fd)
	{
		perror("open");
		return -1;
	}
	
	int ret = 0;

	ret = read(fd, stdin, 5);

	close(fd);

	return 0;
}

