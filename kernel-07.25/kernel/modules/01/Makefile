
obj-m	+=	func.o

KERNEL_DIR := /share/arm/FriendlyARM-4412-201506/FriendlyARM-4412-DVD-A/Linux/linux-3.5
ROOTFS	:= /nfsroot

all:
	make -C $(KERNEL_DIR) M=`pwd` modules

clean:
	make -C $(KERNEL_DIR) M=`pwd` clean

install:
	make -C $(KERNEL_DIR) M=`pwd` modules_install INSTALL_MOD_PATH=$(ROOTFS)
	

