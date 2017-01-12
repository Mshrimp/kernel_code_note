#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

#define	DEV_NAME	"test_dev"

void my_mdelay(unsigned long val)
{
	unsigned long end = jiffies + msecs_to_jiffies(val);
	//unsigned long end = jiffies + (HZ * val / 1000);
	//while(jiffies < end)
	//	;
	while(time_is_after_jiffies(end))
		;
}

int test_init(void)			
{
	printk("module init\n");
	printk("jiffies : %lu\n", jiffies);
	my_mdelay(1000);
	printk("jiffies : %lu\n", jiffies);
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

