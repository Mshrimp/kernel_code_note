#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>

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

	unsigned int col = 0xff;
	int i, j;
	unsigned int *buf_col = malloc(800 * 4);
	while(1)
	{
		for(i = 0; i < 800; i++)
			buf_col[i] = col;

		for(i = 0; i < 480; i++)
		{
			write(fd, buf_col, 800 * 4);
		/*
			for(j = 0; j < 800; j++)
			{
				write(fd, &col, 4);
			}
		*/
		}

		col <<= 8;
		if(col == 0xff000000)
			col = 0xff;
		lseek(fd, 0, SEEK_SET);

		sleep(1);
	}
	close(fd);

#endif
	return 0;
}
