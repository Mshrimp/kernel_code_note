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
#define BELL_NAME	"test-bell"

void *reg_con;
void *reg_dat;

static void bell_init(void)
{
/*
#if 1
	gpio_free(BELL);
#endif
	gpio_request(BELL, BELL_NAME);
	gpio_direction_output(BELL, 0);
*/
	reg_con = ioremap(BEEL_CON_REG, 8);
	reg_dat = ioremap(BEEL_DAT_REG, 8);
	//if ()
	iowrite32((ioread32(reg_con) & (~0xF)) | 0x1, reg_con);
	iowrite32((ioread32(reg_dat) & (~0x1)), reg_dat);
}

static void bell_uninit(void)
{
/*
	gpio_direction_output(BELL, 0);
	gpio_free(BELL);
*/
	iowrite32((ioread32(reg_dat) & (~0x1)), reg_dat);
	iounmap(reg_con);
	iounmap(reg_dat);
}

static void bell_on(void)
{
/*
	gpio_direction_output(BELL, 1);
*/
	printk("Bell on!\n");
	iowrite32(ioread32(reg_dat) | 0x1, reg_dat);
}

static void bell_off(void)
{
/*
	gpio_direction_output(BELL, 0);
*/
	printk("Bell off!\n");
	iowrite32(ioread32(reg_dat) & (~0x1), reg_dat);
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


