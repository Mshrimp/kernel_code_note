#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char ** argv)
{
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s dev_file info\n", argv[0]);
		return -1;
	}

	int fd;
	fd = open(argv[1], O_RDWR);	//open -> sys_open -> test_open 
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}
	printf("fd = %d\n", fd);

	int ret;
	ret = write(fd, argv[2], strlen(argv[2]));	//sys_write <- write
	printf("ret = %d\n", ret);
	close(fd);
	return 0;
}
