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

int fd;
void func(int sig)
{
	printf("pid = %d sig = %d\n", getpid(), sig);
	char buf[1024] = {};
	int ret;
	ret = read(fd, buf, 1024);
	if(ret > 0)
		printf("buf = %s\n", buf);
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s fifo\n", argv[0]);
		return -1;
	}

	if(signal(SIGIO, func) == SIG_ERR)
	{
		perror("signel");
		return -1;
	}

	fd = open(argv[1], O_RDONLY);
	if(-1 == fd)
	{
		perror("open");
		return -1;
	}

	fcntl(fd, F_SETOWN, getpid());		//设置信号发送的进程id;
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK | O_ASYNC);//异步io;	
										//				 ^ fasync();

	while(1)
	{
		sleep(1);
	}

	close(fd);
	return 0;
}



