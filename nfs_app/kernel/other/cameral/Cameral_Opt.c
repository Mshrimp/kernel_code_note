
#include "Cameral_Opt.h"

static char *yuv[3] ;
static int fd ; 
static int length ;

int Init_Cameral(int Width , int Height)
{

	fd = open("/dev/video0" , O_RDWR);
	if(fd < 0)
	{
		perror("open video device fail");
		return -1 ; 
	}
	
	
	//1.设置采取摄像头的图片格式跟图片大小
	struct v4l2_format  format ; 
	//规定格式
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV ; 
	format.fmt.pix.width = Width; 
	format.fmt.pix.height = Height;
	
	if(ioctl(fd , VIDIOC_S_FMT , &format) != 0)
	{
		perror("set video pixel format fail");
		return -1 ; 
	}


	//2.requestbuffer
	//设置摄像头的内存分块信息 
	struct v4l2_requestbuffers   requestbuffer ; 
	requestbuffer.count = 3 ;   //分配3块的空间 
	requestbuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
	requestbuffer.memory = V4L2_MEMORY_MMAP ; 
	
	if(ioctl(fd , VIDIOC_REQBUFS , &requestbuffer ) != 0)
	{
		perror("request buffer fail");
		return -2 ; 
	}

	//3.query buffer  使申请空间排队
	struct v4l2_buffer querybuffer ; 
	
	int i ; 
	for(i = 0 ; i < 3 ; i++)
	{
		querybuffer.index = i ;
		querybuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
		querybuffer.memory = V4L2_MEMORY_MMAP ; 

		if(ioctl(fd , VIDIOC_QUERYBUF , &querybuffer) != 0)	
		{
			perror("query buffer fail");
			return -3 ; 
		}

		printf("i:%d  offset:%d length:%d \n" , querybuffer.index , querybuffer.m.offset , querybuffer.length);
		
	#if 0
       void *mmap(void *start, size_t length, int prot, int flags,
                  int fd, off_t offset);
      
	#endif

        yuv[i] = (char *)mmap(NULL , querybuffer.length , PROT_READ , MAP_SHARED , fd , querybuffer.m.offset);

		//入队
		
		if(ioctl(fd , VIDIOC_QBUF , &querybuffer) != 0)
		{
			perror("queue buffer fail");
			return -4 ; 
		}
	}

	int on = 1 ; 
	//开启报像头 
	if(ioctl(fd , VIDIOC_STREAMON , &on) != 0)
	{
		perror("cameral on fail");
		return -5 ; 
	}

	return 0 ; 
}

int Get_Picture(char *YUV)
{

	//出队一个
	struct v4l2_buffer  dequeue ; 
	dequeue.type = V4L2_BUF_TYPE_VIDEO_CAPTURE ; 
	dequeue.memory = V4L2_MEMORY_MMAP ; 
	
	if(ioctl(fd , VIDIOC_DQBUF , &dequeue) != 0)
	{
		perror("dequeue buffer fail");
		return -6 ;
	}

	memcpy(YUV , yuv[dequeue.index] , dequeue.length );
	length = dequeue.length ; 

	//入队一个
	if(ioctl(fd , VIDIOC_QBUF , &dequeue) != 0)
	{
		perror("dequeue buffer fail");
		return -7 ;
	}
	return 0 ; 
}

void Close_Cameral(void)
{
	int i ; 

	for(i = 0 ; i < 3 ; i++)
	{
		munmap(yuv[i] , length);
	}

	close(fd);
}
