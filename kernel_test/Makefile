obj-m += test_simple.o
obj-m += chardev.o
obj-m += procfs.o
obj-m += seq_fs.o
obj-m += sys-vars.o

kver=6.5.0-28-generic

all:
	make -C /lib/modules/$(kver)/build M=$(PWD) modules
clean:	
	make -C /lib/modules/$(kver)/build M=$(PWD) clean
