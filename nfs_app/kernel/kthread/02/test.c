#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>

#define	DEV_NAME	"test_dev"

int thread_fn(void *data)
{
	int i;
	for(i = 0; i < 3; i++)
	{
		mdelay(1000);
		printk("pid : %u comm : %s\n", current->pid, current->comm);
	}
	return 0;
}

struct task_struct *task;
int test_init(void)			
{
	int i;
	printk("module init\n");
	/*
	task = kthread_create(thread_fn, NULL, "test-thread%d", 0);
	if(IS_ERR(task))
	{
		return PTR_ERR(task);
	}
	wake_up_process(task);
	*/

	for(i = 0; i < 10; i++)
	{
		task = kthread_run(thread_fn, NULL, "test-thread%02d", i);
		if(IS_ERR(task))
			return PTR_ERR(task);
	}

	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

