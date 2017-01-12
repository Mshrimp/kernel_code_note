#ifndef	__IOCTL_BELL_H__
#define __IOCTL_BELL_H__

#include <linux/ioctl.h>

#define BELL_TYPE	0x5

#define BELL_ON		_IO(BELL_TYPE, 0)
#define BELL_OFF	_IO(BELL_TYPE, 1)

#endif
