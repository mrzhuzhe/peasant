/*
/var/log/kern.log

insmod test_simple.ko
rmmod test_simple.ko

*/


#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void){
	printk(KERN_INFO "hello world\n");
	return 0;
}

void cleanup_module(void){
	printk(KERN_INFO "bye bye world\n");
}

MODULE_LICENSE("GPL");
