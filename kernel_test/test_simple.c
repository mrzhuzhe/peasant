/*
https://tldp.org/LDP/lkmpg/2.6/html/x427.html
/var/log/kern.log

insmod test_simple.ko
rmmod test_simple.ko

*/


#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>


short myshort = 1;
int myint = 520;
long mylong = 9999;
char *mychars = "abcd";
int myintArr[2] = {-1, 1};
int arr_arg_c = 0;

__initdata int testvar = 3;
//int testvar = 4;

/* 
 * module_param(foo, int, 0000)
 * The first param is the parameters name
 * The second param is it's data type
 * The final argument is the permissions bits, 
 * for exposing parameters in sysfs (if non-zero) at a later stage.
 */

module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");
module_param(mychars, charp, 0000);
MODULE_PARM_DESC(mychars, "A character string");

/*
 * module_param_array(name, type, num, perm);
 * The first param is the parameter's (in this case the array's) name
 * The second param is the data type of the elements of the array
 * The third argument is a pointer to the variable that will store the number 
 * of elements of the array initialized by the user at module loading time
 * The fourth argument is the permission bits
 */
module_param_array(myintArr, int, &arr_arg_c, 0000);
MODULE_PARM_DESC(myintArr, "An array of integers");

int __init init_module_zz(void){
	int i;
	printk(KERN_INFO "Hello, world 5\n=============\n");
	printk(KERN_INFO "myshort is a short integer: %hd\n", myshort);
	printk(KERN_INFO "myint is an integer: %d\n", myint);
	printk(KERN_INFO "mylong is a long integer: %ld\n", mylong);
	printk(KERN_INFO "mystring is a string: %s\n", mychars);
	for (i = 0; i < (sizeof myintArr / sizeof (int)); i++)
	{
		printk(KERN_INFO "myintArray[%d] = %d\n", i, myintArr[i]);
	}
	printk(KERN_INFO "got %d arguments for myintArray.\n", arr_arg_c);

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
//MODULE_SUPPORTED_DEVICE("testdevice");
