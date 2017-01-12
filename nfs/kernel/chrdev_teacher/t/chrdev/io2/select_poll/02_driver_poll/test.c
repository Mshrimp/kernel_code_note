#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/sched.h>
#include <linux/list.h>
#include <linux/poll.h>

#define DEV_NAME	"test-mode"
wait_queue_head_t wq;	//定义一个等待队列头;
int data_len = 0;
volatile bool empty = true;
int test_open (struct inode *inode, struct file *filp)
{
	int val;
	struct thread_info *thread = (struct thread_info *)((unsigned int)(&val) & ~(8192-1));
	printk("test open\n");
	printk("pid : %d\n", current->pid);
	printk("pid : %d\n", thread->task->pid);

	return 0;
}

int test_close (struct inode *inode, struct file *filp)
{
	printk("test close\n");
	return 0;
}

ssize_t test_read (struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	int len = 0;
	printk("test read\n");

	while (empty == true)
	{
		if(wait_event_interruptible(wq, !empty) == -ERESTARTSYS)//可中断睡眠;
			return -ERESTARTSYS;
	}

	len = size > data_len ? data_len : size;
	data_len -= len;

	if(data_len == 0)
		empty = true;

	printk("read: data_len = %d\n", data_len);
	return size;
}

ssize_t test_write (struct file *filp, const char __user *buf, size_t size, loff_t *offset)
{
	printk("test write\n");

	data_len += size;
	if(data_len > 0)
		empty = false;
	
	wake_up_interruptible(&wq);
	printk("write: data_len = %d\n", data_len);

	return size;
}

unsigned int test_poll (struct file *filp, struct poll_table_struct *ptable)
{
	printk("test select \n");
	poll_wait(filp, &wq, ptable);	//把sys_poll自己加入当前驱动的等待队列;
	
	if(!empty)
		return POLLIN;				//返回有数据可读;
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = test_open,
	.release = test_close,
	.read = test_read, 
	.write = test_write,
	.poll = test_poll,
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

	init_waitqueue_head(&wq);	//初始化等待队列头;

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


