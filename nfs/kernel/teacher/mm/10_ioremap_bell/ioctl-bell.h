#ifndef	__IOCTL_BELL_H__
#define __IOCTL_BELL_H__

#include <linux/ioctl.h>

#define BELL_TYPE	0x5

#define BELL_ON		_IO(BELL_TYPE, 0)
#define BELL_OFF	_IO(BELL_TYPE, 1)

#define GPD0CON		0x114000A0
#define	BEEL_CON_REG	GPD0CON
#define	GPD0DAT		0x114000A4
#define	BEEL_DAT_REG	GPD0DAT


#endif
