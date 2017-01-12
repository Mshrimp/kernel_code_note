
obj-m		+= test.o

KERNEL_DIR := /share/arm/FriendlyARM-4412-201506/FriendlyARM-4412-DVD-A/Linux/linux-3.5
ROOTFS	:= /nfsroot

all:
	make -C $(KERNEL_DIR) M=`pwd` modules

app:
	arm-linux-gcc app.c -o app

clean:
	make -C $(KERNEL_DIR) M=`pwd` clean
	rm -rf app

install:
	make -C $(KERNEL_DIR) M=`pwd` modules_install INSTALL_MOD_PATH=$(ROOTFS)
	

