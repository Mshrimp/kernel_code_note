#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ioctl-bell.h"

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s dev_file\n", argv[0]);
		return -1;
	}

	int fd;
	fd = open(argv[1], O_RDWR);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}

	while(1)
	{
		ioctl(fd, BELL_ON);
		sleep(1);
		ioctl(fd, BELL_OFF);
		sleep(1);
	}

	close(fd);
	return 0;
}



