obj-m += test_simple.o

kver=6.5.0-28-generic

all:
	make -C /lib/modules/$(kver)/build M=$(PWD) modules
clean:	
	make -C /lib/modules/$(kver)/build M=$(PWD) clean
