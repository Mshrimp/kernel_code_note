#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

#define	DEV_NAME	"test_dev"

struct timer_list timer;
//func软中断级别执行:
void func(unsigned long data)
{
	printk("timer out data = %lu\n", data);
	mod_timer(&timer, jiffies + 2 * HZ);
}

int test_init(void)			
{
	printk("module init\n");
	setup_timer(&timer, func, 100);	
	mod_timer(&timer, jiffies + 2 * HZ);
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	del_timer(&timer);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

