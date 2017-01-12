#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/completion.h>

#define	DEV_NAME	"test_dev"

#define KEY0	EXYNOS4_GPX3(2)

struct test_dev{
	char *name;

	int irq_num;
	struct list_head work_fn_head;
	//void (*fn)(char *str);			

	struct task_struct *task;
	struct completion comp; 

	int major;
	struct file_operations fops;
};

spinlock_t lock;
void func(char *str)								//下半部处理中断;
{
	int i;
	for(i = 0; i < 3; i++)
	{
		//msleep(1000);
		mdelay(1000);
		printk("func: [%s] pid : %u comm : %s\n", str, current->pid, current->comm);
	}
}

void test(char *str)								//下半部处理中断;
{
	int i;
	for(i = 0; i < 3; i++)
	{
		msleep(1000);
		//mdelay(1000);
		printk("test: [%s] pid : %u comm : %s\n", str, current->pid, current->comm);
	}
}

struct work_fn{
	void (*fn)(char *str);			
	char *work_data;
	struct list_head list;
};

int thread_fn(void *data)							//下半部;
{
	struct test_dev *tdev = data;
	struct work_fn *wf;
	while(1)
	{
		if(kthread_should_stop())
			break;
		
		wait_for_completion(&tdev->comp);

		//if(tdev->fn)
		if(list_empty(&tdev->work_fn_head))
			mdelay(10);
		else
		{
			wf = list_entry(tdev->work_fn_head.next, struct work_fn, list);
			list_del(tdev->work_fn_head.next);
			wf->fn(wf->work_data);
			//kfree(wf);
		}
	}
	return 0;
}

struct work_fn work;
irqreturn_t key0_handler(int num, void *dev_id)		//响应中断; 上半部;
{
	struct test_dev *tdev = dev_id;
	printk("test key0 handler num = %d\n", num);
	printk("tdev->name = %s\n", tdev->name);
	
	//struct work_fn *work = kmalloc();

	//wake_up_process(tdev->task);
	//tdev->fn = func;
	work.fn = func;
	work.work_data = "test-key0-data";
	list_add_tail(&work.list, &tdev->work_fn_head);
	complete(&tdev->comp);

	return IRQ_HANDLED; 
}

struct work_fn work1;
irqreturn_t key1_handler(int num, void *dev_id)		//响应中断; 上半部;
{
	struct test_dev *tdev = dev_id;
	printk("test key1 handler num = %d\n", num);
	printk("tdev->name = %s\n", tdev->name);

	//tdev->fn = test;
	work1.fn = test;
	work1.work_data = "test-key1-data";
	list_add_tail(&work1.list, &tdev->work_fn_head);
	complete(&tdev->comp);

	return IRQ_HANDLED; 
}

int test_open (struct inode *inode, struct file *filp)
{
	printk("test open\n");	
	func("open");
	return 0;
}

int test_release (struct inode *inode, struct file *filp)
{
	printk("test release\n");
	return 0;
}

struct test_dev tdev = {
	.name = "testkey0",
	.major = 0,
	.fops = {
		.owner = THIS_MODULE,
		.open = test_open,
		.release = test_release,
	},
};

int test_init(void)			
{
	int ret;
	printk("module init\n");
	ret = request_irq(IRQ_EINT(26), key0_handler, 
				IRQF_TRIGGER_FALLING, "test-key0", &tdev);
	if(IS_ERR_VALUE(ret))
		goto ERROR_irq0;

	ret = request_irq(IRQ_EINT(27), key1_handler,
				IRQF_TRIGGER_FALLING, "test-key1", &tdev);
	if(IS_ERR_VALUE(ret))
		goto ERROR_irq1;

	ret = register_chrdev(tdev.major, DEV_NAME, &tdev.fops);
	if(IS_ERR_VALUE(ret))
		goto ERROR_register;

	if(tdev.major == 0)
	{
		tdev.major = ret;
	}
	printk("major : %d \n", tdev.major);

	spin_lock_init(&lock);
	init_completion(&tdev.comp);
	INIT_LIST_HEAD(&tdev.work_fn_head);
	tdev.task = kthread_run(thread_fn, &tdev, "test-key-thread%02d", 0);
	if(IS_ERR(tdev.task))
	{
		ret = PTR_ERR(tdev.task);
		goto ERROR_kthread;
	}

	return 0;
ERROR_kthread:
	unregister_chrdev(tdev.major, DEV_NAME); 
ERROR_register:
	free_irq(IRQ_EINT(27), &tdev);
ERROR_irq1:
	free_irq(IRQ_EINT(26), &tdev);
ERROR_irq0:
	return ret;
}

void test_exit(void)
{
	int ret;
	printk("cleanup module\n");
	//wake_up_process(tdev.task);
	//tdev.fn = NULL;
	complete(&tdev.comp);
	ret = kthread_stop(tdev.task);
	printk("ret = %d\n", ret);
	unregister_chrdev(tdev.major, DEV_NAME); 
	free_irq(IRQ_EINT(27), &tdev);
	free_irq(IRQ_EINT(26), &tdev);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

