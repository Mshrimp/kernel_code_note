#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include "ioctl-bell.h"

#define DEV_NAME	"test-mode"

static int val;
module_param(val, int, 0);
static int major;
module_param(major, int, 0);

#define BELL	EXYNOS4_GPD0(0)
#define GPD0CON		0x114000A0
#define BELL_REG	GPD0CON
#define BELL_NAME	"test-bell"

void *reg;

static void bell_init(void)
{
/*
#if 1
	gpio_free(BELL);
#endif
	gpio_request(BELL, BELL_NAME);
	gpio_direction_output(BELL, 0);
*/
	reg = ioremap(BELL_REG, 8);
	printk("reg = %p\n", reg);
	iowrite32(ioread32(reg) &~ 0xf, reg);
	iowrite32(ioread32(reg) | 0x1, reg);

	iowrite32(ioread32(reg+4) &~ 0x1, reg+4);
}

static void bell_uninit(void)
{
/*
	gpio_direction_output(BELL, 0);
	gpio_free(BELL);
*/
	iowrite32(ioread32(reg+4) &~ 0x1, reg+4);
	iounmap(reg);
}

static void bell_on(void)
{
/*
	gpio_direction_output(BELL, 1);
*/
	printk("bell_on \n");
	iowrite32(ioread32(reg+4) | 0x1, reg+4);

}

static void bell_off(void)
{
/*
	gpio_direction_output(BELL, 0);
*/
	printk("bell_off \n");
	iowrite32(ioread32(reg+4) &~ 0x1, reg+4);
}

int test_open (struct inode *inode, struct file *filp)
{
	printk("test open\n");

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
	
	ret = register_chrdev(major, DEV_NAME, &fops);
	if(ret < 0)
		return ret;
	
	if(major == 0)	//
		major = ret;

	printk("major = %d\n", major);

	return 0;
}

static void __exit test_exit(void)
{
	printk("test exit\n");
	unregister_chrdev(major, DEV_NAME);
}

module_init(test_init);
module_exit(test_exit);

MODULE_LICENSE("GPL");


