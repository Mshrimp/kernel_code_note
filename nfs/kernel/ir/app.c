#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ioctl_bell.h"

#define PWM_IOCTL_SET_FREQ      1
#define PWM_IOCTL_STOP          0

//#include "ioctl_led.h"

int main(int argc, char ** argv)
{
#if 0
	while(1)
		;
#endif

#if 1
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

	char buf[100] = {};
	while(1)
	{
		read(fd, buf, 100);
		printf("buf = %s\n", buf);
		sleep(1);
		memset(buf, 0, 100);
	}

	close(fd);
#endif
	return 0;
}
