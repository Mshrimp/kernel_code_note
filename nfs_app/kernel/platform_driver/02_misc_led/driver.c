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
#include "ioctl_led.h"

#define	DEV_NAME	"test_dev"

struct test_led_dev{
	void *reg;
	int reg_len;
	
	struct file_operations fops;
	struct miscdevice mdev;
};

void led_init(struct test_led_dev *tdev)
{
	iowrite32(ioread32(tdev->reg) &~ 0xffff, tdev->reg);
	iowrite32(ioread32(tdev->reg) | 0x1111, tdev->reg);

	iowrite32(ioread32(tdev->reg + 4) | 0xf, tdev->reg + 4);
}

void led_uninit(struct test_led_dev *tdev)
{
	iowrite32(ioread32(tdev->reg + 4) | 0xf, tdev->reg + 4);
}

void led_all_on(struct test_led_dev *tdev)
{
	iowrite32(ioread32(tdev->reg + 4) &~ 0xf, tdev->reg + 4);
}

void led_on(struct test_led_dev *tdev, unsigned long on)
{
	iowrite32(ioread32(tdev->reg + 4) &~ (1<<on), tdev->reg + 4);
}

void led_all_off(struct test_led_dev *tdev)
{
	iowrite32(ioread32(tdev->reg + 4) | 0xf, tdev->reg + 4);
}

void led_off(struct test_led_dev *tdev, unsigned long on)
{
	iowrite32(ioread32(tdev->reg + 4) | (1<<on), tdev->reg + 4);
}

long test_ioctl (struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct test_led_dev *tdev = filp->private_data;
	printk("test ioctl\n");
	if(_IOC_TYPE(cmd) != LED_TYPE)
		return -EINVAL;
	
	switch(cmd)
	{
		case LED_ALL_ON:
			led_all_on(tdev);
			break;
		case LED_ALL_OFF:
			led_all_off(tdev);
			break;
		case LED_ON:
			led_on(tdev, arg);
			break;
		case LED_OFF:
			led_off(tdev, arg);
			break;
		default:
			return -EINVAL;
	}

	return 0;
}

int test_open (struct inode *inode, struct file *filp)
{
	struct test_led_dev *tdev 
			= container_of(filp->f_op, struct test_led_dev, fops); 
	printk("test open\n");
	filp->private_data = tdev;	
	led_init(tdev);
	return 0;
}

int test_release (struct inode *inode, struct file *filp)
{
	struct test_led_dev *tdev = filp->private_data;
	printk("test release \n");
	led_uninit(tdev);
	return 0;
}

int test_probe (struct platform_device *dev)		//插入;
{
	int ret;
	struct test_led_dev *tdev;
	struct resource *res;
	printk("test probe driver start\n");
	
	tdev = kzalloc(sizeof(struct test_led_dev), GFP_KERNEL);
	if(IS_ERR_OR_NULL(tdev))
	{
		ret = -ENOMEM;
		goto ERROR_kzalloc;
	}

	platform_set_drvdata(dev, tdev);

	res = platform_get_resource(dev, IORESOURCE_MEM, 0);	//
	if(IS_ERR_OR_NULL(res))
	{
		ret = -ENOMEM;
		goto ERROR_iomem;
	}

	tdev->reg_len = resource_size(res);
	tdev->reg = ioremap(res->start, tdev->reg_len);
	if(IS_ERR_OR_NULL(tdev->reg))
	{
		ret = -ENOMEM;
		goto ERROR_ioremap;
	}

	tdev->fops.owner = THIS_MODULE;
	tdev->fops.open = test_open;
	tdev->fops.release = test_release;
	tdev->fops.unlocked_ioctl = test_ioctl;

	tdev->mdev.name = DEV_NAME;
	tdev->mdev.minor = MISC_DYNAMIC_MINOR;
	tdev->mdev.fops = &tdev->fops;
	ret = misc_register(&tdev->mdev);
	if(IS_ERR_VALUE(ret))
	{
		goto ERROR_misc;
	}

	return 0;
ERROR_misc:
	iounmap(tdev->reg);
ERROR_ioremap:
ERROR_iomem:
	kfree(tdev);
ERROR_kzalloc:
	return ret;
}

int test_remove (struct platform_device *dev)	//拔出;
{
	struct test_led_dev *tdev;
	printk("test remove driver end\n");
	tdev = platform_get_drvdata(dev);
	misc_deregister(&tdev->mdev);
	iounmap(tdev->reg);
	kfree(tdev);
	return 0;
}

struct platform_device_id idtable[] = {		//当前驱动支持的设备列表;
	{"test-dev", 0},
	{"test-dev1", 1},
	{},
};

struct platform_driver tdri = {
	.driver = {
		.name = "test-dri", 	
	},
	.probe = test_probe,
	.remove = test_remove,
	.id_table = idtable,
};

int test_init(void)			
{
	int ret;
	printk("module init\n");
	ret = platform_driver_register(&tdri);
	if(IS_ERR_VALUE(ret))
	{
		return ret;
	}
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	platform_driver_unregister(&tdri);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

