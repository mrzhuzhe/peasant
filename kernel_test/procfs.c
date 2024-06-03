/*
https://sysprog21.github.io/lkmpg/#the-proc-file-system 
a better version
*/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/version.h>

#define procfs_name "hellofatass"

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

/* Put data into the proc fs file.
 * 
 * Arguments
 * =========
 * 1. The buffer where the data is to be inserted, if
 *    you decide to use it.
 * 2. A pointer to a pointer to characters. This is
 *    useful if you don't want to use the buffer
 *    allocated by the kernel.
 * 3. The current position in the file
 * 4. The size of the buffer in the first argument.
 * 5. Write a "1" here to indicate EOF.
 * 6. A pointer to data (useful in case one common 
 *    read for multiple /proc/... entries)
 *
 * Usage and Return Value
 * ======================
 * A return value of zero means you have no further
 * information at this time (end of file). A negative
 * return value is an error condition.
 *
 * For More Information
 * ====================
 * The way I discovered what to do with this function
 * wasn't by reading documentation, but by reading the
 * code which used it. I just looked to see what uses
 * the get_info field of proc_dir_entry struct (I used a
 * combination of find and grep, if you're interested),
 * and I saw that  it is used in <kernel source
 * directory>/fs/proc/array.c.
 *
 * If something is unknown about the kernel, this is
 * usually the way to go. In Linux we have the great
 * advantage of having the kernel source code for
 * free - use it.
 */

struct proc_dir_entry *Our_Proc_File; 

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





