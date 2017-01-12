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
	struct timeval time;
	printk("module init\n");
	do_gettimeofday(&time);
	printk("tv_sec : %ld tv_usec : %ld\n", time.tv_sec, time.tv_usec);
	printk("jiffies : %lu\n", jiffies);
	
	//mdelay(1);
	udelay(1);

	printk("jiffies : %lu\n", jiffies);
	do_gettimeofday(&time);
	printk("tv_sec : %ld tv_usec : %ld\n", time.tv_sec, time.tv_usec);
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

