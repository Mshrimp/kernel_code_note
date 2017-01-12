#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "ioctl_led.h"

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
	//printf("fd = %d\n", fd);

#if 0 
	while(1)
	{
		ioctl(fd, LED_ALL_ON);
		sleep(1);
		ioctl(fd, LED_ALL_OFF);
		sleep(1);
	}
#endif
#if 1
	unsigned long i = 0;
	struct led_op op;
	//for (i = 0; i < 4; i++)	
	while(1)
	{
		op.num = i;
		op.on = 1;
		//ioctl(fd, LED_ALL_ON);
		ioctl(fd, LED_ON);
		//ioctl(fd, LED_SET, &op);
		sleep(1);
		ioctl(fd, LED_OFF);
		op.on = 0;
		//ioctl(fd, LED_SET, &op);
		sleep(1);
		i++;
		if(i == 4)
			i = 0;
	}
#endif

	close(fd);
	return 0;
}
