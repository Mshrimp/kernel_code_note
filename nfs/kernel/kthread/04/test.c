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
	//int i;
	while(1)
	//for(i = 0; i < 3; i++)
	{
		if(kthread_should_stop())	//死循环不会自己结束, 发送stop结束; 检测stop信号;
			break;

		mdelay(2000);
		printk("pid : %u comm : %s\n", current->pid, current->comm);
	}
	return 0;
}

struct task_struct *task;
int test_init(void)			
{
	printk("module init\n");
#if 1
	task = kthread_create(thread_fn, NULL, "test-thread%d", 0);
	if(IS_ERR(task))
	{
		return PTR_ERR(task);
	}
	wake_up_process(task);
#else
	task = kthread_run(thread_fn, NULL, "test-thread%02d", 0);
	if(IS_ERR(task))
		return PTR_ERR(task);
#endif
	return 0;
}

void test_exit(void)
{
	int ret;
	printk("cleanup module\n");
	while(1)
	{
		ret = kthread_stop(task);
		if(ret == -EINTR)
		{
			printk("ret = %d\n", ret);
			//wake_up_process(task);
			break;
		}
		else
		{
			printk("ret = %d\n", ret);
			break;
		}
	}

}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

