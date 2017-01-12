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
	struct timespec old;
	struct timespec new;
	printk("module init\n");
	getnstimeofday(&old);
	printk("tv_sec : %ld tv_nsec : %ld\n", old.tv_sec, old.tv_nsec);
	printk("jiffies : %lu\n", jiffies);
	
	mdelay(1);
	//udelay(500);

	printk("jiffies : %lu\n", jiffies);
	getnstimeofday(&new);
	printk("tv_sec : %ld tv_nsec : %ld\n", new.tv_sec, new.tv_nsec);
	printk("nsec : %ld\n", new.tv_nsec - old.tv_nsec);
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

