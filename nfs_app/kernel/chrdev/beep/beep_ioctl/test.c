#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include "ioctl_beep.h"

#define DEV_BEEP_NAME	"beep_test-module"

struct beep_test_dev
{
	unsigned int gpio;
	int major;
	struct file_operations fops;
};

///////////////////////裸板驱动//////////////////////////

#define BEEP	EXYNOS4_GPD0(0)

void beep_init(struct beep_test_dev *tdev)
{	
	gpio_free(tdev->gpio);
	gpio_request(tdev->gpio, "test_beep");
}

void beep_uninit(struct beep_test_dev *tdev)
{
	gpio_free(tdev->gpio);
}

void beep_on(struct beep_test_dev *tdev)
{
	gpio_direction_output(tdev->gpio, 1);
}

void beep_off(struct beep_test_dev *tdev)
{
	gpio_direction_output(tdev->gpio, 0);
}


/////////////////////字符设备框架///////////////////////

int beep_test_open(struct inode *inode, struct file *filp)
{
	struct beep_test_dev *tdev;
	printk("beep test open!\n");
	tdev = container_of(filp->f_op, struct beep_test_dev, fops);
	beep_init(tdev);
	beep_off(tdev);

	return 0;
}

int beep_test_release(struct inode *inode, struct file *filp)
{
	struct beep_test_dev *tdev;
	printk("beep test release!\n");
	tdev = container_of(filp->f_op, struct beep_test_dev, fops);
	beep_off(tdev);
	beep_uninit(tdev);

	return 0;
}

long beep_test_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	struct beep_test_dev *tdev;
	tdev = container_of(filp->f_op, struct beep_test_dev, fops);
	printk("beep test ioctl!\n");
	
	switch(cmd)
	{
		case BEEP_ON:
			printk("Beep on!\n");
			beep_on(tdev);
			break;
		case BEEP_OFF:
			printk("Beep off!\n");
			beep_off(tdev);
			break;
	}

	return 0;
}



/////////////////////////模块/////////////////////////////

struct beep_test_dev tdev = {
	.gpio = BEEP,
	.major = 0,
	.fops = {
		.owner = THIS_MODULE,
		.open = beep_test_open,
		.release = beep_test_release,
		.unlocked_ioctl = beep_test_ioctl,
	},
};

int beep_test_init(void)
{
	int ret = 0;
	printk("module beep init!\n");

	ret = register_chrdev(tdev.major, DEV_BEEP_NAME, &tdev.fops);
	if(ret < 0)
	{
		printk("register failed!\n");
		return ret;	
	}

	if (tdev.major == 0)
	{
		tdev.major = ret;
		printk("major = %d\n", tdev.major);
	}
	else
	{
		printk("major = %d\n", tdev.major);
	}

	return 0;
}

void beep_test_exit(void)
{
	printk("cleanup module!\n");
	unregister_chrdev(tdev.major, DEV_BEEP_NAME);
}

module_init(beep_test_init);
module_exit(beep_test_exit);
MODULE_LICENSE("GPL");

