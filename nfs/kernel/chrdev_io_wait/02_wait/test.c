#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/list.h>
#include "ioctl-bell.h"

#define DEV_NAME	"test-mode"

volatile bool empty;
int test_open (struct inode *inode, struct file *filp)
{
	int val;
	struct thread_info *thread = (struct thread_info *)((unsigned int)(&val) & ~(8192-1));
	//char buf[1024 * 8];
	//memset(buf, 0xff, 1024 * 8);
	printk("test open\n");
	printk("pid : %d\n", current->pid);
	printk("pid : %d\n", thread->task->pid);

	empty = true;

	return 0;
}

int test_close (struct inode *inode, struct file *filp)
{
	printk("test close\n");
	return 0;
}

struct task_queue{
	struct task_struct *task;
	struct list_head list_head;
};

LIST_HEAD(head);

struct task_struct *task;
ssize_t test_read (struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	printk("test read\n");

	//while(empty == true)
	//	;	
	if (empty)
	{
		//task = current;
		struct task_queue task;
		task.task = current;
		list_add_tail(&task.list_head, &head);

		//set_current_state(TASK_UNINTERRUPTIBLE);
		set_current_state(TASK_INTERRUPTIBLE);
		schedule();
	}

	empty = true;
	return size;
}

ssize_t test_write (struct file *filp, const char __user *buf, size_t size, loff_t *offset)
{
	struct task_queue *task;
	printk("test write\n");

	empty = false;
	if(!list_empty(&head))
	{
		task = list_entry(head.next, struct task_queue, list_head);
		list_del(head.next);
		wake_up_process(task->task);
	}

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


