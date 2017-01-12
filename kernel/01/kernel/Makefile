

all:
	./conf --silentoldconfig Kconfig
	gcc -I ./include code/test.c -o vmlinux

menuconfig:
	./mconf Kconfig

clean:
	rm -rf vmlinux code/test.o

