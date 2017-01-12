#ifndef	__IOCTL_WDT_H__
#define __IOCTL_WDT_H__

#include <linux/ioctl.h>

#define WDT_TYPE		0x1

#define WDT_START			_IO(WDT_TYPE, 0)
#define WDT_STOP			_IO(WDT_TYPE, 1)
#define	WDT_KEEPALIVE		_IO(WDT_TYPE, 2)
#define WDT_TIMER_OUT		_IOW(WDT_TYPE, 3, unsigned long)
#define WDT_IRQ				_IO(WDT_TYPE, 4)

#endif

