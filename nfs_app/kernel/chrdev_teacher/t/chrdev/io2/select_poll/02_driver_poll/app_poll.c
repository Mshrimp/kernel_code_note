#include <stdio.h>
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

void func(int sig)
{
	printf("sig = %d\n", sig);
}

int main(int argc, char **argv)
{
	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s fifo1 fifo2\n", argv[0]);
		return -1;
	}

	if(signal(SIGINT, func) == SIG_ERR)
	{
		perror("signel");
		return -1;
	}

	int fd, fd2;
	fd = open(argv[1], O_RDWR);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}

	fd2 = open(argv[2], O_RDWR);
	if(-1 == fd2)
	{
		perror("open");
		return -1;
	}

	char buf1[100] = {};
	char buf2[100] = {};
	int ret;

	int max;
	struct pollfd fds[2] = {};
	fds[0].fd = fd;
	fds[0].events = POLLIN;
	fds[1].fd = fd2;
	fds[1].events = POLLIN;

	while(1)
	{
		ret = poll(fds, 2, -1);		
		if(-1 == ret)
		{
			perror("poll");
			return -1;
		}
		else if(0 == ret)
		{
			printf("time out\n");
		}
		else if(ret > 0)
		{
			if(fds[0].revents == POLLIN)
			{
				ret = read(fd, buf1, 10);
				printf("buf1 : %s \n", buf1);
			}

			if(fds[1].revents == POLLIN)
			{
				ret = read(fd2, buf2, 10);
				printf("buf2 : %s \n", buf2);
			}
		}
	}

	close(fd);
	return 0;
}



