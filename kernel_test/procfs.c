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
	/* 
	 * We give all of our information in one go, so if the
	 * user asks us if we have more information the
	 * answer should always be no.
	 *
	 * This is important because the standard read
	 * function from the library would continue to issue
	 * the read system call until the kernel replies
	 * that it has no more information, or until its
	 * buffer is filled.
	 */
	if(*offset || procfs_buffer_size == 0){
	
	}
	char s[] = "Hello mfs\n";
	int len = sizeof(s);
	ssize_t ret = len;
	
	if (*offset >= len || copy_to_user(buffer, s, len)){
		pr_info("copy to  user failed\n");
		ret=0;
	} else {
		pr_info("procfile read %s\n", file_pointer->f_path.dentry->d_name.name);
		*offset += len;
	}
	return ret;
}

#ifdef HAVE_PROC_OPS
const struct proc_ops proc_file_fops = {
	.proc_read = procfile_read,
};
#else 
const struct file_operations proc_file_fops = {
	.read = procfile_read,
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
	printk(KERN_INFO "/proc/%s created\n", procfs_name);
	return 0;
}	


void cleanup_module()
{
	proc_remove(Our_Proc_File);
	pr_info(KERN_INFO "/proc/%s removed\n", procfs_name);
}



MODULE_LICENSE("GPL");





