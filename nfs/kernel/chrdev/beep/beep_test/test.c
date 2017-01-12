#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include "beep_ioctl.h"

#define		BEEP_DEV_NAME	"Beep_test_module"

struct beep_dev
{
	unsigned int gpio;
	int major;
	struct file_operations fops;
};

////////////////////////裸板驱动////////////////////////

#define		BEEP	EXYNOS4_GPD0(0)

void beep_gpio_init(struct beep_dev *tdev)
{
	gpio_free(tdev->gpio);
	gpio_request(tdev->gpio, "beep_test_gpio");
}

void beep_gpio_uninit(struct beep_dev *tdev)
{
	gpio_free(tdev->gpio);
}

void beep_on(struct beep_dev *tdev)
{
	gpio_direction_output(tdev->gpio, BEEP_ON);
}

void beep_off(struct beep_dev *tdev)
{
	gpio_direction_output(tdev->gpio, BEEP_OFF);
}

///////////////////////字符设备////////////////////////

int beep_dev_open(struct inode *inode_p, struct file *file_p)
{
	struct beep_dev *tdev;
	tdev = container_of(file_p->f_op, struct beep_dev, fops);
	printk("beep_dev_open!\n");

	beep_gpio_init(tdev);
	beep_off(tdev);

	return 0;
}

int beep_dev_release(struct inode *inode_p, struct file *file_p)
{
	struct beep_dev *tdev;
	tdev = container_of(file_p->f_op, struct beep_dev, fops);
	printk("beep_dev_release!\n");

	beep_off(tdev);
	beep_gpio_uninit(tdev);

	return 0;
}

int beep_dev_ioctl(struct file *file_p, unsigned int cmd, unsigned long args)
{
	struct beep_dev *tdev;
	tdev = container_of(file_p->f_op, struct beep_dev, fops);
	printk("beep_dev_ioctl!\n");

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

///////////////////////模块//////////////////////

struct beep_dev tdev = {
	.gpio = BEEP,
	.major = 0,
	.fops = {
		.open = beep_dev_open,
		.release = beep_dev_release,
		.unlocked_ioctl = beep_dev_ioctl,
	},
};

int beep_init(void)
{
	int ret = 0;

	printk("Module beep test init!\n");

	ret = register_chrdev(tdev.major, BEEP_DEV_NAME, &tdev.fops);
	if (ret < 0)
	{
		printk("register chrdev failed!\n");
		return -1;
	}

	if (0 == tdev.major)
	{
		tdev.major = ret;
		printk("major = 0, major = %d\n", tdev.major);
	}
	else
	{
		printk("major != 0, major = %d\n", tdev.major);
	}

	return 0;
}

void beep_exit(void)
{
	printk("Module beep test cleanup!\n");
	unregister_chrdev(tdev.major, BEEP_DEV_NAME);
}


module_init(beep_init);
module_exit(beep_exit);
MODULE_LICENSE("GPL");




