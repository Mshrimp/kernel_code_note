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
//1)key0 irq -> func
//2)func     -> open (preempt)
void func(unsigned long data)
{
	int i;
	for(i = 0; i < 3; i++)
	{
		printk("timer out data = %lu\n", data);
		mdelay(1000);
		//msleep(1);
	}
	timer.expires = jiffies + 2 * HZ;
	add_timer(&timer);
}

int test_init(void)			
{
	printk("module init\n");
	setup_timer(&timer, func, 100);	
	timer.expires = jiffies + 2 * HZ;
	add_timer(&timer);
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

