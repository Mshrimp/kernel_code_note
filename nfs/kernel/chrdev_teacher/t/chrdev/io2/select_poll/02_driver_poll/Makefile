
obj-m	+= test.o func.o

KERNEL_SRC	:= /share/arm/FriendlyARM-4412-201506/FriendlyARM-4412-DVD-A/Linux/linux-3.5
#KERNEL_SRC	:= /lib/modules/`uname -r`/build/
ROOTFS := /nfsroot

all:
	make -C $(KERNEL_SRC) M=`pwd` modules

app:
	arm-linux-gcc app.c -o app

install:
	make -C $(KERNEL_SRC) M=`pwd` modules_install INSTALL_MOD_PATH=$(ROOTFS)
	
clean:
	make -C $(KERNEL_SRC) M=`pwd` clean
	rm -rf app







