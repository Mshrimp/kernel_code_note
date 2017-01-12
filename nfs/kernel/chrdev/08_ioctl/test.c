#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include "ioctl_led.h"

#define	DEV_NAME	"test-mode"

struct test_dev
{
	unsigned int gpio[4];
	int major;
	struct file_operations fops;	
};

///////////////////////裸板驱动//////////////////////////

#define LED1	EXYNOS4X12_GPM4(0)
#define LED2	EXYNOS4X12_GPM4(1)
#define LED3	EXYNOS4X12_GPM4(2)
#define LED4	EXYNOS4X12_GPM4(3)

void led_init(struct test_dev *tdev)
{
	int i;
#if 1
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_free(tdev->gpio[i]);	
#endif

	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_request(tdev->gpio[i], "test-led");	
}

void led_uninit(struct test_dev *tdev)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_free(tdev->gpio[i]);	
}

void led_all_on(struct test_dev *tdev)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_direction_output(tdev->gpio[i], 0);
}

void led_on(struct test_dev *tdev, unsigned long on)
{
	gpio_direction_output(tdev->gpio[on], 0);
}

void led_all_off(struct test_dev *tdev)
{
	int i;
	for(i = 0; i < ARRAY_SIZE(tdev->gpio); i++)
		gpio_direction_output(tdev->gpio[i], 1);
}

void led_off(struct test_dev *tdev, unsigned long on)
{
	gpio_direction_output(tdev->gpio[on], 1);
}


/////////////////////字符设备框架///////////////////////

int num;
//驱动open实现:
//inode : 文件信息; 	同一个文件不变;
//filp	: 一次打开标志;	不同次打开文件不同;
int test_open (struct inode *inode, struct file *filp)
{
	struct test_dev *tdev;
	printk("test open\n");
	tdev = container_of(filp->f_op, struct test_dev, fops);
	led_init(tdev);
	led_all_off(tdev);
	num = MINOR(inode->i_rdev);
	return 0;
}

//驱动close实现:
//inode : 文件信息;
//filp	: 一次打开标志;
int test_release (struct inode *inode, struct file *filp)
{
	struct test_dev *tdev;
	printk("test release\n");
	tdev = container_of(filp->f_op, struct test_dev, fops);
	led_all_off(tdev);
	led_uninit(tdev);
	return 0;
}

//驱动ioctl实现;
//filp: 文件指针;
//cmd : 请求命令;
//args: 命令参数;
long test_ioctl (struct file *filp, unsigned int cmd, unsigned long args)
{
	struct test_dev *tdev;
	struct led_op *op;
	tdev = container_of(filp->f_op, struct test_dev, fops);
	printk("test ioctl\n");

	if(_IOC_TYPE(cmd) != LED_TYPE)
		return -EINVAL;

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
		case LED_ON:
			printk("led %d on \n", num);
			led_on(tdev, num);
			break;
		case LED_OFF:
			printk("led %d off\n", num);
			led_off(tdev, num);
			break;
		case LED_SET:
			op = (void *)args;
			printk("led %d %s\n", op->num, op->on ? "on" : "off");
			if(op->on)
				led_on(tdev, op->num);
			else
				led_off(tdev, op->num);
			break;
		default:
			return -EINVAL;
	}
	return 0;
}

/////////////////////////模块/////////////////////////////
struct test_dev tdev = {
	.gpio = {
		LED1, LED2, LED3, LED4
	},
	.major = 0,
	.fops = {
		.owner = THIS_MODULE,		//引用计数; 
		.open = test_open,			//可选实现;
		.release = test_release,	//可选实现;
		.unlocked_ioctl = test_ioctl,
	},
};

int test_init(void)			
{
	int ret = 0;
	printk("module init\n");
	
	//1. 创建并注册一个字符设备到内核; cdev
	ret = register_chrdev(tdev.major, DEV_NAME, &tdev.fops);
	if(ret < 0)
	{
		return ret;
	}
	if(tdev.major == 0)		//向内核动态申请; 
	{
		tdev.major = ret;	//返回值就是主设备号;
		printk("major = %d\n", tdev.major);
	}
	else					//向内核静态申请;
	{
		printk("major = %d\n", tdev.major);
	}

	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	unregister_chrdev(tdev.major, DEV_NAME);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

