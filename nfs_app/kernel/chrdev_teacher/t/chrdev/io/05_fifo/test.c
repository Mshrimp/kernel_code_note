#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>

#define MAX_SIZE	1024		
char test_buf[MAX_SIZE] = {};
loff_t start = 0;
loff_t end = 0;

ssize_t test_read (struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	ssize_t ret = 0;
	int i;
	size_t count = 0;
	size_t len = 0;
	//device -> kernel -> user
	printk("test read\n");

	if(start == end)
		return 0;

	if(end > start)
	{
		count = end - start;
	}
	else 
	{
		count = MAX_SIZE - (start - end) - 1;
	}
	
	if(size > count)
		size = count;

	for(i = 0; i < size; i++)
	{
		ret = copy_to_user(buf + i, test_buf + start, 1);	//返回没有copy成功的个数;
		start += 1 - ret;
		len += 1 - ret;
		start = start % MAX_SIZE;
	}
	return len;
}

ssize_t test_write (struct file *filp, const char __user *buf, size_t size, loff_t *offset)
{
	ssize_t ret = 0;
	size_t count = 0;
	size_t len = 0;
	int i;
	//user -> kernel -> device
	printk("test wirte\n");

	if((end + 1) % MAX_SIZE == start)
		return 0;

	if(start > end)
	{
		count = start - end;		
	}
	else
	{
		count = MAX_SIZE - (end - start);	
	}

	if(count < size)
		size = count;

	for(i = 0; i < size; i++)
	{
		ret = copy_from_user(test_buf + end, buf + i, 1);
		end += 1 - ret;
		len += 1 - ret;
		end = end % MAX_SIZE;
	}
	return len;
}

int test_open (struct inode *inode, struct file *filp)
{
	printk("test open\n");
	return 0;
}

int test_release (struct inode *inode, struct file *filp)
{
	printk("test release\n");
	return 0;
}

loff_t test_llseek (struct file *filp, loff_t offset, int whence)
{
	loff_t cur = filp->f_pos;
	printk("test llseek\n");
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

