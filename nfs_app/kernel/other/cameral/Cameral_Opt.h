
#ifndef  _CAMERAL_OPT_H
#define  _CAMERAL_OPT_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/videodev2.h>


int Init_Cameral(int Width , int Height);

int Get_Picture(char *YUV);

void Close_Cameral(void);

#endif  //_CAMERAL_OPT_H 
