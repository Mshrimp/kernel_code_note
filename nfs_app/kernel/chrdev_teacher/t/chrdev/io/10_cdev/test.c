#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include "ioctl-bell.h"

#define DEV_NAME	"test-mode"

static int val;
module_param(val, int, 0);
static int major = 0;
module_param(major, int, 0);
static char *name;
module_param(name, charp, 0);
static int minor = 10;
static int count = 5;
static dev_t devno;
static struct cdev *cdev = NULL;

#define BELL	EXYNOS4_GPD0(0)
#define BELL_NAME	"test-bell"

static void bell_init(void)
{
#if 1
	gpio_free(BELL);
#endif
	gpio_request(BELL, BELL_NAME);
	gpio_direction_output(BELL, 0);
}

static void bell_uninit(void)
{
	gpio_direction_output(BELL, 0);
	gpio_free(BELL);
}

static void bell_on(void)
{
	gpio_direction_output(BELL, 1);
}

static void bell_off(void)
{
	gpio_direction_output(BELL, 0);
}

int test_open (struct inode *inode, struct file *filp)
{
	printk("test open\n");
	printk("major(%d) minor(%d)\n", MAJOR(inode->i_rdev), MINOR(inode->i_rdev));
	bell_init();
	return 0;
}

int test_close (struct inode *inode, struct file *filp)
{
	printk("test close\n");
	
	bell_uninit();
	return 0;
}

long test_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
	printk("test ioctl\n");

	if(_IOC_TYPE(cmd) != BELL_TYPE)
		return -EINVAL;

	switch(cmd)
	{
		case BELL_ON:
			bell_on();
			break;
		case BELL_OFF:
			bell_off();
			break;
		default:
			return -EINVAL;
	}

	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = test_open,
	.release = test_close,
	.unlocked_ioctl = test_ioctl,
};

static int __init test_init(void)
{
	int ret;
	printk("test init\n");	
	printk("val = %d\n", val);
	printk("name = %s\n", name);
	
#if 0
	ret = register_chrdev(major, DEV_NAME, &fops);
	if(ret < 0)
		return ret;
	
	if(major == 0)	//
		major = ret;

	printk("major = %d\n", major);
#endif

	if(major != 0)	//静态申请;
	{
		devno = MKDEV(major, minor);
		//1. 申请设备号, 申请成功返回0;
		//	 major : 55  minor : 10 - 14
		ret = register_chrdev_region(devno, count, DEV_NAME);
		if(IS_ERR_VALUE(ret))
			goto ERROR_register;
	}
	else			//动态申请;
	{
		ret = alloc_chrdev_region(&devno, minor, count, DEV_NAME);
		if(ret != 0)
			goto ERROR_register;
		major = MAJOR(devno);
		minor = MINOR(devno);
	}
	//2. 创建空的struct cdev;
	cdev = cdev_alloc();	
	if(IS_ERR_OR_NULL(cdev))
	{
		ret = -ENOMEM;
		goto ERROR_cdev_alloc;
	}

	//3. 初始化cdev;
	cdev_init(cdev, &fops);
	cdev->owner = THIS_MODULE;
	//4. 加入cdev到系统中;
	ret = cdev_add(cdev, devno, count);
	if(IS_ERR_VALUE(ret))
		goto ERROR_cdev_add;

	printk("major = %d minor = %d\n", major, minor);
	return 0;

ERROR_cdev_add:
	cdev_del(cdev);
ERROR_cdev_alloc:
	unregister_chrdev_region(devno, count);
ERROR_register:
	return ret;
}

static void __exit test_exit(void)
{
	printk("test exit\n");
	//unregister_chrdev(major, DEV_NAME);
	cdev_del(cdev);
	unregister_chrdev_region(devno, count);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");


