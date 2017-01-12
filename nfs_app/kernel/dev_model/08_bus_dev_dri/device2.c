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

extern struct bus_type mini;
void dev_release(struct device *dev)
{
}
struct device tdev = {
	.init_name = "test-dev2",
	.bus = &mini,
	.release = dev_release,
};

int test_init(void)			
{
	int ret;
	printk("module init\n");
	ret = device_register(&tdev);
	if(IS_ERR_VALUE(ret))
	{
		return ret;
	}

	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	device_unregister(&tdev);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

