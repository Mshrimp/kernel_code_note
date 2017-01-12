#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>

#define MAX_SIZE	1024		
char test_buf[MAX_SIZE] = {};

ssize_t test_read (struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	int ret;
	//device -> kernel -> user
	printk("test read\n");
	
	//memcpy(buf, test_buf + *offset, size);
	ret = copy_to_user(buf, test_buf + *offset, size);	//返回没有copy成功的个数;
	*offset += size - ret;

	return size - ret;
}

ssize_t test_write (struct file *filp, const char __user *buf, size_t size, loff_t *offset)
{
	//user -> kernel -> device
	printk("test wirte\n");

	//memcpy(test_buf + *offset, buf, size);
	ret = copy_from_user(test_buf + *offset, buf, size);
	*offset += size - ret;

	return size - ret;
}

//open -> sys_open 
//	   <- fd
//	   <- struct file *filp
//	   		filp->f_pos = 0;
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

loff_t test_llseek (struct file *filp, loff_t offset, int whence)
{
	printk("test llseek\n");
	loff_t cur = filp->f_pos;
	switch(whence)
	{
		case SEEK_SET:
			cur = offset;
			break;
		case SEEK_CUR:
			cur += offset;
			break;
		case SEEK_END:
			cur = MAX_SIZE + offset;
			break;
	}

	if(cur > MAX_SIZE)
		cur = MAX_SIZE;

	filp->f_pos = cur;
	return cur;
}

struct file_operations fops = {
	.owner = THIS_MODULE,		//引用计数; 
	.open = test_open,			//可选实现;
	.release = test_release,	//可选实现;
	.read = test_read,
	.write = test_write,
	.llseek = test_llseek,		
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

