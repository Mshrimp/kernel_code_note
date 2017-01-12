#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/timer.h>

#define	DEV_NAME	"test_dev"

void func(unsigned long data)
{
	struct task_struct *task = (void *)data;
	wake_up_process(task);	
}

void my_msleep(unsigned long ms)
{
	struct timer_list timer;
	struct task_struct *task = current;
	setup_timer(&timer, func, (unsigned long)task);
	mod_timer(&timer, jiffies + msecs_to_jiffies(ms));

	set_current_state(TASK_UNINTERRUPTIBLE);
	schedule();

	del_timer(&timer);
}

int test_init(void)			
{
	printk("module init\n");
	printk("jiffies = %lu\n", jiffies);
	my_msleep(1000);
	printk("jiffies = %lu\n", jiffies);
	my_msleep(2000);
	printk("jiffies = %lu\n", jiffies);
	my_msleep(5000);
	printk("jiffies = %lu\n", jiffies);
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

