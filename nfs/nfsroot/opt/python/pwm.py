#!/usr/bin/python
import fcntl
fd = open('/dev/pwm', 'r')
fcntl.ioctl(fd, 1, 100)
