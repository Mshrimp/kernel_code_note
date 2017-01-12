
obj-m	+=  func.o
obj-m	+=	test.o

KERNEL_DIR := /home/xiami/note/FriendlyARM/Linux/linux-3.5
ROOTFS := /nfsroot_mini

all:
	make -C $(KERNEL_DIR) M=`pwd` modules

clean:
	make -C $(KERNEL_DIR) M=`pwd` clean

install:
	make -C $(KERNEL_DIR) M=`pwd` modules_install INSTALL_MOD_PATH=$(ROOTFS)



