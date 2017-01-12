#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char ** argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s dev_file\n", argv[0]);
		return -1;
	}

	int fd;
	fd = open(argv[1], O_RDWR);	//open -> sys_open -> test_open 
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}

	char ch;
	int ret;
	ret = read(fd, &ch, 1);
	printf("ret = %d\n", ret);

	close(fd);
	return 0;
}
