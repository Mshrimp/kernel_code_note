#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/device.h>
#include "mini.h"

#define	DEV_NAME	"test_dev"

#define GPM4CON     0x110002E0
#define GPM4DAT     0x110002E4


struct resource res[] = {
	[0] = {
		.start = GPM4CON,
		.end = GPM4CON + 7, 
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = IRQ_EINT(26),
		.end = IRQ_EINT(29),
		.flags = IORESOURCE_IRQ,
	},
};

struct mini_device tdev = {
	.name = "test-dev1",
	.resources = res,
	.res_num = ARRAY_SIZE(res),
};

int test_init(void)			
{
	int ret;
	printk("module init\n");
	ret = mini_device_register(&tdev);
	if(IS_ERR_VALUE(ret))
	{
		return ret;
	}

	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	mini_device_unregister(&tdev);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

