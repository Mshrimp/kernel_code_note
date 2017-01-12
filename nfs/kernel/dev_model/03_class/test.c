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

struct class tclass = {
	.name = "test_class",
	.owner = THIS_MODULE,
};

int test_init(void)			
{
	int ret;
	printk("module init\n");
	ret = class_register(&tclass);
	if(IS_ERR_VALUE(ret))
		return ret;
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	class_unregister(&tclass);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

