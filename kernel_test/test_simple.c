/*
/var/log/kern.log

insmod test_simple.ko
rmmod test_simple.ko

*/


#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

__initdata int testvar = 3;
//int testvar = 4;

int __init init_module_zz(void){
	printk(KERN_INFO "hello world %d\n", testvar);
	return 0;
}

void __exit cleanup_module_zz(void){
	//printk(KERN_INFO "bye bye world %d\n", testvar);
	printk(KERN_INFO "bye bye world \n");
}


module_init(init_module_zz);
module_exit(cleanup_module_zz);
MODULE_LICENSE("GPL");

/*
 * Or with defines, like this:
 */
MODULE_AUTHOR("mrzz");	/* Who wrote this module? */
MODULE_DESCRIPTION("Just another test driver");	/* What does this module do */

/*  
 *  This module uses /dev/testdevice.  The MODULE_SUPPORTED_DEVICE macro might
 *  be used in the future to help automatic configuration of modules, but is 
 *  currently unused other than for documentation purposes.
 */
MODULE_SUPPORTED_DEVICE("testdevice");
