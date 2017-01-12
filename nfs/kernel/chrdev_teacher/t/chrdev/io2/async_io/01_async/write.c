#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <poll.h>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s fifo\n", argv[0]);
		return -1;
	}

	int fd;
	fd = open(argv[1], O_WRONLY);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}

	write(fd, "hello", 5);

	close(fd);
	return 0;
}



