#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>

ssize_t test_read (struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	printk("test read\n");
	return size;
}

ssize_t test_write (struct file *filp, const char __user *buf, size_t size, loff_t *offset)
{
	printk("test wirte\n");
	return size;
}

int test_open (struct inode *inode, struct file *filp)
{
	//try_module_get(&__this_module);		//加引用计数;
	printk("test open\n");
	return 0;
}

int test_release (struct inode *inode, struct file *filp)
{
	//module_put(&__this_module);			//减引用计数;
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

	ret = register_chrdev(major, "test-mode", &fops);
	if(ret < 0)
	{
		return ret;
	}
	if(major == 0)
	{
		major = ret;
		printk("major = %d\n", major);
	}
	else
	{
		printk("major = %d\n", major);
	}

	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	unregister_chrdev(major, "test-mode");
}

module_init(test_init);
module_exit(test_exit);
MODULE_LICENSE("GPL");

