#!/bin/sh

case $MDEV in
sda1)
	DEVNAME=sda1
	MOUNTPOINT=/udisk/0
	;;
sdb1)
	DEVNAME=sdb1
	MOUNTPOINT=/udisk/1
	;;
sdcard | mmcblk1p1)
	DEVNAME=sdcard
	MOUNTPOINT=/sdcard
	;;
*)
	exit 0
	;;
esac

test ! -c /dev/null && mknod -m 0666 /dev/null c 1 3

case $ACTION in
remove)
	/bin/umount $MOUNTPOINT || true
	rmdir $MOUNTPOINT >/dev/null 2>&1 || true
	;;
*)
	/bin/mkdir -p $MOUNTPOINT > /dev/null 2>&1 || true
	/bin/mount -t vfat -o codepage=936 -o iocharset=utf8 -o sync -o noatime -o nodiratime /dev/$DEVNAME $MOUNTPOINT > /dev/null 2>&1 || true
	;;
esac

exit 0
