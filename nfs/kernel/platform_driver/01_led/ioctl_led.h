#ifndef	__IOCTL_LED_H__
#define __IOCTL_LED_H__

#include <linux/ioctl.h>

#define LED_TYPE		0x1

#define LED_ALL_ON		_IO(LED_TYPE, 0)
#define LED_ALL_OFF		_IO(LED_TYPE, 1)
#define	LED_ON			_IOW(LED_TYPE, 2, unsigned long)
#define LED_OFF			_IOW(LED_TYPE, 3, unsigned long)
//#define	LED_ON			_IO(LED_TYPE, 2)
//#define LED_OFF			_IO(LED_TYPE, 3)
#define LED_SET			_IOW(LED_TYPE, 4, struct led_op)

struct led_op{
	int num;
	int on;
};


#endif

