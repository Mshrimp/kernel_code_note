#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

#define	DEV_NAME	"test_dev"

int test_init(void)			
{
	printk("module init\n");
	printk("jiffies : %lu\n", jiffies);
	msleep(1000);
	//ssleep(1);
	//mdelay(1000);
	//udelay(1000);
	//udelay(1000);
	//udelay(1000);
	//udelay(1000);
	//udelay(1000);
	//ndelay(1000000*5);
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

