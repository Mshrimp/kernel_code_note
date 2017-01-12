#include <linux/fs.h>
#include <linux/module.h>




#define TEST_DEV_NAME		"test_dev_wait"

volatile bool empty = true;

int test_open(struct inode *inode, struct file *filp)
{
	printk("test open!\n");

	return 0;
}


ssize_t test_read(struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	printk("test read!\n");

	return 0;
}

ssize_t test_write(struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	printk("test write!\n");

	return 0;
}

int test_close(struct inode *inode, struct file *filp)
{
	printk("test close!\n");

	return 0;
}



struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = test_open,
	.read = test_read,
	.write = test_write,
	.release = test_close,
};


int major = 0;

static int __init test_init(void)
{
	int ret = 0;

	printk("test init!\n");

	ret = register_chrdev(major, TEST_DEV_NAME, &fops);
	if (ret < 0)
	{
		return ret;
	}

	if (major == 0)
	{
		major = ret;
	}
	
	printk("major = %d\n", major);

	return 0;	
}


static void __exit test_exit(void)
{
	printk("test exit!\n");
	unregister_chrdev(major, TEST_DEV_NAME);
	
	return;
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");

