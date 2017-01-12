#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>

#define	DEV_NAME	"test_dev"

int test_probe (struct platform_device *dev)		//插入;
{
	int i;
	printk("test probe driver start\n");
	for(i = 0; i < dev->num_resources; i++)
	{
		if(dev->resource[i].flags == IORESOURCE_IRQ)
		{
			//request_irq();
			printk("IRQ RESOURCE: start(%d) end(%d)\n", 
				dev->resource[i].start, 
				dev->resource[i].end);
		}
		if(dev->resource[i].flags == IORESOURCE_MEM)
		{
			//ioremap();
			printk("MEM RESOURCE: start(%x) end(%x)\n",
				dev->resource[i].start, 
				dev->resource[i].end);
		}
	}
	return 0;
}

int test_remove (struct platform_device *dev)	//拔出;
{
	printk("test remove driver end\n");
	return 0;
}

struct platform_driver tdri = {
	.driver = {
		.name = "test-dev", 	
	},
	.probe = test_probe,
	.remove = test_remove,
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

