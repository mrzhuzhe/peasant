/*
https://sysprog21.github.io/lkmpg/#the-proc-file-system 
a better version
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 10, 0)
#include <linux/minmax.h>
#endif

#define procfs_name "hellofatass"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

#define PROCFS_MAX_SIZE 2048UL
#define PROCFS_ENTRY_FILENAME "buffer2k"

struct proc_dir_entry *Our_Proc_File; 
char procfs_buffer[PROCFS_MAX_SIZE];
unsigned long procfs_buffer_size = 0;


ssize_t procfile_read(struct file *file_pointer, char __user *buffer, size_t buffer_length, loff_t *offset)
{
	printk(KERN_INFO "procfile_read (/proc/%s) called\n", procfs_name);
	if (*offset || procfs_buffer_size == 0) { 
       		pr_debug("procfs_read: END\n"); 
        	*offset = 0; 
        	return 0; 
    	} 
   	procfs_buffer_size = min(procfs_buffer_size, buffer_length); 
    	if (copy_to_user(buffer, procfs_buffer, procfs_buffer_size)) 
        	return -EFAULT; 
   	*offset += procfs_buffer_size; 
   	pr_debug("procfs_read: read %lu bytes\n", procfs_buffer_size); 
   	return procfs_buffer_size; 
}

ssize_t procfile_write(struct file *file, const char  __user *buffer, size_t len, loff_t *off){
	procfs_buffer_size = min(PROCFS_MAX_SIZE, len);
	if (copy_from_user(procfs_buffer, buffer, procfs_buffer_size))	
		return -EFAULT;
	*off += procfs_buffer_size;
	pr_debug("procfs_write: write %lu bytes\n", procfs_buffer_size);
	return procfs_buffer_size;
}

static int procfs_open(struct inode *inode, struct file *file) 
{ 
    try_module_get(THIS_MODULE); 
    return 0; 
} 

static int procfs_close(struct inode *inode, struct file *file) 
{ 
    module_put(THIS_MODULE); 
    return 0; 
} 


#ifdef HAVE_PROC_OPS
const struct proc_ops proc_file_fops = {
	.proc_read = procfile_read,
	.proc_write = procfile_write, 
   	.proc_open = procfs_open, 	
   	.proc_release = procfs_close, 
};
#else 
const struct file_operations proc_file_fops = {
	.read = procfile_read,
	.proc_write = procfile_write, 
   	.proc_open = procfs_open, 	
   	.proc_release = procfs_close, 
};
#endif


int init_module()
{
	Our_Proc_File = proc_create(procfs_name, 0644, NULL, &proc_file_fops);
	if (Our_Proc_File == NULL){
		proc_remove(Our_Proc_File);
		printk(KERN_ALERT, "Error: Could not initialize /proc/%s\n", procfs_name);
		return -ENOMEM;
	}	
 	proc_set_size(Our_Proc_File, 80); 
   	proc_set_user(Our_Proc_File, GLOBAL_ROOT_UID, GLOBAL_ROOT_GID); 
	printk(KERN_INFO "/proc/%s created\n", procfs_name);
	return 0;
}	


void cleanup_module()
{
	proc_remove(Our_Proc_File);
	pr_info(KERN_INFO "/proc/%s removed\n", procfs_name);
}



MODULE_LICENSE("GPL");





