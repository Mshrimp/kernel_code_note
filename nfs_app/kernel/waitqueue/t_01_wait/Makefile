obj-m	+=	 test.o

KERNEL_SRC := /home/xiami/note/FriendlyARM/Linux/linux-3.5
ROOTFS := /nfsroot_mini


all:
	make -C $(KERNEL_SRC) M=`pwd` modules

app:
	arm-linux-gcc app.c -o app

clean:
	make -C $(KERNEL_SRC) M=`pwd` clean
	rm app -rf



