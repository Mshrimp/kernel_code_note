#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>

#define	DEV_NAME	"test_dev"

struct semaphore sem;
ssize_t test_read (struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	printk("test read\n");
	return size;
}

ssize_t test_write (struct file *filp, const char __user *buf, size_t size, loff_t *offset)
{
	printk("test write\n");
	return size;
}

void func(void)
{
	int i;
	down(&sem);
	for(i = 0; i < 3; i++)
	{
		mdelay(1000);
		printk("pid : %u comm : %s\n", current->pid, current->comm);
	}
	up(&sem);
}

int test_open (struct inode *inode, struct file *filp)
{
	printk("test open\n");
	func();
	return 0;
}

int test_release (struct inode *inode, struct file *filp)
{
	printk("test release\n");
	return 0;
}

struct file_operations fops = {
		.owner = THIS_MODULE,		//引用计数; 
		.open = test_open,			//可选实现;
		.release = test_release,	//可选实现;
		.read = test_read,
		.write = test_write,
};

int major = 0;
int test_init(void)			
{
	int ret = 0;
	printk("module init\n");
	
	//1. 创建并注册一个字符设备到内核; cdev
	ret = register_chrdev(major, DEV_NAME, &fops);
	if(ret < 0)
	{
		return ret;
	}
	if(major == 0)		//向内核动态申请; 
	{
		major = ret;	//返回值就是主设备号;
		printk("major = %d\n", major);
	}
	else					//向内核静态申请;
	{
		printk("major = %d\n", major);
	}

	//sema_init(&sem, 0);
	sema_init(&sem, 1);

	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	unregister_chrdev(major, DEV_NAME);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

