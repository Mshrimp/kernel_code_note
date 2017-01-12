#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/device.h>

#define	DEV_NAME	"test_dev"

struct class tclass = {
	.name = "test_class",
	.owner = THIS_MODULE,
};

void dev_release (struct device *dev)
{
	printk("dev release\n");
}

struct device tdev = {
	.init_name = DEV_NAME,
	.class = &tclass,
	.release = dev_release,
};

int major = 0;
struct file_operations fops = {
	.owner = THIS_MODULE,
};
int test_init(void)			
{
	int ret;
	printk("module init\n");
	ret = class_register(&tclass);
	if(IS_ERR_VALUE(ret))
		goto ERROR_class;

	ret = register_chrdev(major, DEV_NAME, &fops);
	if(IS_ERR_VALUE(ret))
	{
		goto ERROR_chrdev;	
	}
	if(major == 0)
	{
		major = ret;
	}

	tdev.devt = MKDEV(major, 0);
	ret = device_register(&tdev);
	if(IS_ERR_VALUE(ret))
	{
		goto ERROR_device;
	}

	return 0;
	
ERROR_device:
	unregister_chrdev(major, DEV_NAME);
ERROR_chrdev:
	class_unregister(&tclass);
ERROR_class:
	return ret;
}

void test_exit(void)
{
	printk("cleanup module\n");
	device_unregister(&tdev);
	unregister_chrdev(major, DEV_NAME);
	class_unregister(&tclass);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

