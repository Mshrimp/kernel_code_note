#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include "ioctl-bell.h"

#define DEV_NAME	"test-mode"

int data_len = 0;
volatile bool empty = true;
int test_open (struct inode *inode, struct file *filp)
{
	int val;
	struct thread_info *thread = (struct thread_info *)((unsigned int)(&val) & ~(8192-1));
	//char buf[1024 * 8];
	//memset(buf, 0xff, 1024 * 8);
	printk("test open\n");
	printk("current->pid : %d\n", current->pid);
	printk("thread->task->pid : %d\n", thread->task->pid);

	//empty = true;

	return 0;
}

int test_close (struct inode *inode, struct file *filp)
{
	printk("test close\n");
	return 0;
}

struct task_struct *task = NULL;
ssize_t test_read (struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	int len = 0;
	printk("test read\n");

	//while(empty == true)
	//	;	
	while (empty)
	{
		task = current;
		//set_current_state(TASK_UNINTERRUPTIBLE);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();

		if(signal_pending(current))
			return -ERESTARTSYS;	//重新调用系统调用;
			//return -EINTR;
			//return -EAGAIN;

		printk("wake up\n");
	}

	len = size > data_len ? data_len : size;
	data_len -= len;

	if(data_len == 0)
		empty = true;
	return size;
}

ssize_t test_write (struct file *filp, const char __user *buf, size_t size, loff_t *offset)
{
	printk("test write\n");

	data_len += size;
	if(data_len > 0)
	{
		empty = false;
	}

	if(task)
		wake_up_process(task);

	return size;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = test_open,
	.release = test_close,
	.read = test_read, 
	.write = test_write,
};

int major = 0;
static int __init test_init(void)
{
	int ret;
	printk("test init\n");	
	ret = register_chrdev(major, DEV_NAME, &fops);
	if(ret < 0)
		return ret;
	
	if(major == 0)	
		major = ret;

	printk("major = %d\n", major);
	return 0;
}

static void __exit test_exit(void)
{
	printk("test exit\n");
	unregister_chrdev(major, DEV_NAME);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");


