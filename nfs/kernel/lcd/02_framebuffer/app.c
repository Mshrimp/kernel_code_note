#include <stdio.h>
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

	struct fb_var_screeninfo var;
	ioctl(fd, FBIOGET_VSCREENINFO, &var);
	struct fb_fix_screeninfo fix;
	ioctl(fd, FBIOGET_FSCREENINFO, &fix);

	printf("xres: %d yres: %d bpp: %d\n", var.xres, var.yres, var.bits_per_pixel);
	printf("smem_start: %p smem_len: %d line_length: %d\n",
			fix.smem_start, fix.smem_len, fix.line_length);


	unsigned int (*p)[800] = mmap(NULL, fix.smem_len, PROT_READ|PROT_WRITE, 
								MAP_SHARED, fd, 0);
	if(NULL == p)
	{
		perror("mmap");
		return -1;
	}
	unsigned int col = 0xff;
	int i, j;
	while(1)
	{
		for(i = 0; i < 480; i++)
		{
			for(j = 0; j < 800; j++)
			{
				p[i][j] = col;
			}
		}
		
		col <<= 8;
		if(col == 0xff000000)
			col = 0xff;

		usleep(1000000);
	}
	munmap(p, fix.smem_len);
	close(fd);
#endif
	return 0;
}
