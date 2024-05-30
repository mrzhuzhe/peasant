#include "chardev.h"

int Major;
int Device_Open=0;
char msg[BUF_LEN];
char *msg_Ptr;

struct file_operations fops = {
	.read=device_read,
	.write=device_write,
	.open=device_open,
	.release=device_release
};

int init_module(void){
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if (Major < 0){
		printk(KERN_ALERT "Registering char device failed with %d\n", Major);
		return Major;
	}
	
	printk(KERN_INFO "I was assigned major number %d. To talk to\n", Major);
	printk(KERN_INFO "the driver, create a dev file with\n");
	printk(KERN_INFO "'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, Major);
	printk(KERN_INFO "Try various minor numbers. Try to cat and echo to\n");
	printk(KERN_INFO "the device file.\n");
	printk(KERN_INFO "Remove the device file and module when done.\n");

	return SUCCESS;	
}

void cleanup_module(void)
{
	unregister_chrdev(Major, DEVICE_NAME);
	//int ret = unregister_chrdev(Major, DEVICE_NAME);
	///if (ret < 0){
	//	printk(KERN_ALERT "Error in unregister_chrdev %d\n", ret);
	//}
}


static int counter = 0;
int device_open(struct inode *inode, struct file *file){
	if (Device_Open){
		return -EBUSY;
	}
	Device_Open++;
	sprintf(msg, "Global static counter %d \n", counter++);
	msg_Ptr=msg;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

int device_release(struct inode *inode, struct file *file)
{
	Device_Open--;		/* We're now ready for our next caller */
	module_put(THIS_MODULE);
	return 0;
}


ssize_t device_read(struct file *flip, char *buffer, size_t length, loff_t *offset){
	int bytes_read=0;
	if (*msg_Ptr == 0){
		return 0;
	}
	while (length && *msg_Ptr){
		put_user(*(msg_Ptr++), buffer++);
		length--;
		bytes_read++;	
	}
	return bytes_read;
}

ssize_t device_write(struct file *flip, const char *buff, size_t len, loff_t *off){
	printk(KERN_ALERT "Sorry this operation not supported\n");
	return -EINVAL;
}


MODULE_LICENSE("GPL");
