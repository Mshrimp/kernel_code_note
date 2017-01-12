#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "Cameral_Opt.h"
#include "j-yuv.h"

#define  WIDTH  640
#define  HEIGHT 480
#define  FRAMERATE  10

int  Width ;
int  Height ;
int  FrameRate ;
int PictureCount ;
char YUV[WIDTH*HEIGHT*2];
char RGB[WIDTH*HEIGHT*3 + 54];

int take_picture()
{
	int fd;
	char filename[20] = {};
	sprintf(filename, "%d.bmp", PictureCount++);
	fd = creat(filename, 0755);
	if(fd == -1)
	{
		perror("creat");
		return -1;
	}
    Get_Picture(YUV);
    yuyv2rgb24((u8 *)YUV , (u8 *)(RGB+54) , Width , Height);
    write(fd, (char *)RGB , Width*Height*3+54);
    close(fd);
}


#define  NR(x)  (sizeof(x)/sizeof(x[0]))

#define   UP   		 0x0001
#define   DOWN		 0x0002
#define   ENTER		 0x0003
#define   UNKNOWN    0x0004
#define   ESC        0x0005

void showmenu(char **buffer , int size , int index);
int getuserinput(int *index , int size);


int main(int argc, char *argv[])
{
	int index = 0 ; 
	int type;
	char *buffer[]={
	"＊       抓拍      ＊",
	"＊    超声波抓拍   ＊",
	};

    Width = WIDTH ;
    Height = HEIGHT;
    FrameRate = FRAMERATE; //帧率
    PictureCount = 0 ;
    Init_Cameral(Width , Height);
    set_bmp_header((struct bmp_header_t *)RGB , Width , Height);

	int i;
	for(i = 0; i < 10; i++)
    	Get_Picture(YUV);
	printf("\033[?25l");

	while(1)
	{
		showmenu(buffer , NR(buffer) , index);
		type = getuserinput(&index , NR(buffer));

		if(type == ESC)
		{
			printf("\033[?25h");
			break;
		}
		if(type == ENTER)
		{
			//index  0 1  
			switch(index)
			{
				case 0:
					take_picture();
					break;
				case 1:
					//super_take_picture();
					break;
			}
		}
	}
	
	return 0;
}

void showmenu(char **buffer , int size , int index)
{
	int i ;
	system("clear");
	for(i = 0 ; i < size ; i++)
	{
		if(index == i)
		printf("\033[%d;30H\033[30m\033[47m%s\033[0m" , i+5 , buffer[i]);
		else
		printf("\033[%d;30H\033[37m\033[40m%s\033[0m" , i+5 , buffer[i]);
	}
	fflush(stdout);
}

static int _getuserinput(void)
{
	int type = UNKNOWN ; 
	char ch ; 
	system("stty -echo");
	system("stty -icanon");
	ch = getchar();
	if(ch == 'q')
		type = ESC ; 
	if(ch == '\n')
		type = ENTER ; 
	if(ch == '\033' && getchar() == '[')
	{
		switch(getchar())
		{
			case 'A'  : type = UP ; break;
			case 'B'  : type = DOWN ; break;
		}
	}
	system("stty echo");
	system("stty icanon");
	return type ;
}

int getuserinput(int *index , int size)
{
	int type ;
	type = _getuserinput();

	switch(type)
	{
		case UP :  if(*index > 0) *index-=1 ; break;
		case DOWN: if(*index < size -1) *index +=1 ; break;
		case ENTER: return ENTER ; 
		case ESC :  return ESC ; 
	}
	return 0 ;
}
