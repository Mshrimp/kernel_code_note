#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/pwm.h>
#include "ioctl_bell.h"

#include <plat/gpio-cfg.h>

#define	DEV_NAME	"test_dev"

struct test_bell_dev {	
	struct pwm_device *pwm;
	struct file_operations fops;
	struct miscdevice mdev;
};

long test_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct test_bell_dev *tdev = filp->private_data;
	printk("test ioctl\n");
	if(_IOC_TYPE(cmd) != BELL_TYPE)
		return -EINVAL;
	
	switch(cmd)
	{
		case BELL_START:
			pwm_enable(tdev->pwm);
			break;
		case BELL_STOP:
			pwm_disable(tdev->pwm);
			break;
		case SET_PWM:
			pwm_config(tdev->pwm, arg/2, arg);
			break;
		default:
			return -EINVAL;
	}

	return 0;
}

int test_open (struct inode *inode, struct file *filp)
{
	struct test_bell_dev *tdev 
			= container_of(filp->f_op, struct test_bell_dev, fops); 
	printk("test open\n");
	filp->private_data = tdev;	

	s3c_gpio_cfgpin(EXYNOS4_GPD0(0), S3C_GPIO_SFN(2));
	return 0;
}

int test_release (struct inode *inode, struct file *filp)
{
	struct test_bell_dev *tdev = filp->private_data;
	printk("test release \n");

	pwm_disable(tdev->pwm);
	s3c_gpio_cfgpin(EXYNOS4_GPD0(0), S3C_GPIO_SFN(0));
	return 0;
}

struct test_bell_dev tdev = {
	.fops = {
		.owner = THIS_MODULE,
		.open = test_open,
		.release = test_release,
		.unlocked_ioctl = test_ioctl,
	},
	.mdev = {
		.name = DEV_NAME,
		.minor = MISC_DYNAMIC_MINOR,
		.fops = &tdev.fops,
	},
};

int test_init(void)			
{
	int ret;
	printk("module init\n");

	tdev.pwm = pwm_request(0, DEV_NAME);
	if(IS_ERR_OR_NULL(tdev.pwm))
	{
		printk("pwm request\n");
		return PTR_ERR(tdev.pwm);
	}

	ret = misc_register(&tdev.mdev);
	if(IS_ERR_VALUE(ret))
	{
		return ret;
	}
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	pwm_free(tdev.pwm);
	misc_deregister(&tdev.mdev);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

