#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

void func(int sig)
{
	printf("sig = %d\n", sig);
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s dev_file\n", argv[0]);
		return -1;
	}

	if(signal(SIGINT, func) == SIG_ERR)
	{
		perror("signel");
		return -1;
	}

	int fd;
	fd = open(argv[1], O_RDWR);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}

	printf("pid : %d\n", getpid());

	int ret;
	while(1)
	{
		ret = read(fd, 0, 5);
		if(-1 == ret)
		{
			if(errno == EAGAIN)
				continue;
			else
			{
				perror("open");
				return -1;
			}
		}
		break;
	}
	close(fd);
	return 0;
}



