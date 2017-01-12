#ifndef	__IOCTL_BELL_H__
#define __IOCTL_BELL_H__

#include <linux/ioctl.h>

#define BELL_TYPE		0x1

#define BELL_START			_IO(BELL_TYPE, 0)
#define BELL_STOP			_IO(BELL_TYPE, 1)
#define SET_PWM				_IOW(BELL_TYPE, 2, unsigned long)

#endif

