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

int test_probe (struct device *dev)		//插入;
{
	printk("test probe driver start\n");
	return 0;
}

int test_remove (struct device *dev)	//拔出;
{
	printk("test remove driver end\n");
	return 0;
}

extern struct bus_type mini;
struct device_driver tdri = {
	.name = "test-dev", 	
	.bus = &mini,
	.probe = test_probe,
	.remove = test_remove,
};

int test_init(void)			
{
	int ret;
	printk("module init\n");
	ret = driver_register(&tdri);
	if(IS_ERR_VALUE(ret))
	{
		return ret;
	}
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	driver_unregister(&tdri);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

