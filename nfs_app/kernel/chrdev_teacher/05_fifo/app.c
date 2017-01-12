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
	printf("fd = %d\n", fd);

	int ret;
	ret = write(fd, "hello", 5);	//sys_write <- write
									//sys_write return -> -ERRNO -> write -> return -1 
	printf("ret = %d\n", ret);
	//char buf[100] = {};
	char *buf = NULL;
	lseek(fd, 0, SEEK_SET);
	ret = read(fd, buf, 5);		//sys_read	<- read
	printf("ret = %d buf = %s\n", ret, buf);

	close(fd);
	return 0;
}
