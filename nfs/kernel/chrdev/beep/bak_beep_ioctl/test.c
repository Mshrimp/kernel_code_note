#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include "ioctl_led.h"
#include "ioctl_beep.h"

#define	DEV_LED_NAME	"led_test-module"
#define DEV_BEEP_NAME	"beep_test-module"

struct led_test_dev
{
	unsigned int gpio[4];

	int major;
	struct file_operations fops;	
};

struct beep_test_dev
{
	unsigned int gpio;

	int major;
	struct file_operations fops;
};

///////////////////////裸板驱动//////////////////////////

#define LED1	EXYNOS4X12_GPM4(0)
#define LED2	EXYNOS4X12_GPM4(1)
#define LED3	EXYNOS4X12_GPM4(2)
#define LED4	EXYNOS4X12_GPM4(3)
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

void led_init(struct led_test_dev *tdev)
{
	int i;
#if 1
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_free(tdev->gpio[i]);	
#endif

	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_request(tdev->gpio[i], "test-led");	
}

void led_uninit(struct led_test_dev *tdev)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_free(tdev->gpio[i]);	
}

void led_all_on(struct led_test_dev *tdev)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_direction_output(tdev->gpio[i], 0);
}

void led_all_off(struct led_test_dev *tdev)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_direction_output(tdev->gpio[i], 1);
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

int led_test_open (struct inode *inode, struct file *filp)
{
	struct led_test_dev *tdev;
	printk("test open\n");
	tdev = container_of(filp->f_op, struct led_test_dev, fops);
	led_init(tdev);
	led_all_off(tdev);
	return 0;
}

int led_test_release (struct inode *inode, struct file *filp)
{
	struct led_test_dev *tdev;
	printk("test release\n");
	tdev = container_of(filp->f_op, struct led_test_dev, fops);
	led_uninit(tdev);
	led_all_off(tdev);
	return 0;
}

long led_test_ioctl (struct file *filp, unsigned int cmd, unsigned long args)
{
	struct led_test_dev *tdev;
	tdev = container_of(filp->f_op, struct led_test_dev, fops);
	printk("test ioctl\n");
	switch(cmd)
	{
		case LED_ALL_ON:
			printk("led all on\n");
			led_all_on(tdev);
			break;
		case LED_ALL_OFF:
			printk("led all off\n");
			led_all_off(tdev);
			break;
	}
	return 0;
}


/////////////////////////模块/////////////////////////////
struct led_test_dev led_tdev = {
	.gpio = {
		LED1, LED2, LED3, LED4
	},
	.major = 0,
	.fops = {
		.owner = THIS_MODULE,		//引用计数; 
		.open = led_test_open,			//可选实现;
		.release = led_test_release,	//可选实现;
		.unlocked_ioctl = led_test_ioctl,
	},
};

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

int led_test_init(void)
{
	int ret = 0;
	printk("module init\n");

	ret = register_chrdev(tdev.major, DEV_LED_NAME, &tdev.fops);
	if(ret < 0)
	{
		return ret;
	}
	if(tdev.major == 0)
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

void led_test_exit(void)
{
	printk("cleanup module\n");
	unregister_chrdev(tdev.major, DEV_LED_NAME);
}

module_init(beep_test_init);
module_exit(beep_test_exit);
MODULE_LICENSE("GPL");

