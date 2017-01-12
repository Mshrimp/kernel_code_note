#include <stdio.h>
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

/*
	ioctl(fd, PWM_IOCTL_SET_FREQ, 1000);
	sleep(1);
	ioctl(fd, PWM_IOCTL_SET_FREQ, 2000);
	sleep(1);	
	ioctl(fd, PWM_IOCTL_SET_FREQ, 3000);
	sleep(1);	
	ioctl(fd, PWM_IOCTL_SET_FREQ, 4000);
	sleep(1);	
	ioctl(fd, PWM_IOCTL_SET_FREQ, 5000);
	ioctl(fd, PWM_IOCTL_SET_FREQ, 1);
	sleep(1);	
	ioctl(fd, PWM_IOCTL_STOP);
*/

	ioctl(fd, SET_PWM, 100000000);

	ioctl(fd, BELL_START);

	sleep(5);

	ioctl(fd, BELL_STOP);

	close(fd);
#endif
	return 0;
}
