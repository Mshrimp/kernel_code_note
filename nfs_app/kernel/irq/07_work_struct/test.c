#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kthread.h>
#include <linux/completion.h>
#include <linux/jiffies.h>

#define	DEV_NAME	"test_dev"

#define KEY0	EXYNOS4_GPX3(2)

struct test_dev{
	char *name;

	int major;
	struct file_operations fops;
};

spinlock_t lock;
void func(struct work_struct *data)								//下半部处理中断;
{
	int i;
	//spin_lock(&lock);
	for(i = 0; i < 3; i++)
	{
		//msleep(1000);
		mdelay(1000);
		printk("func: pid : %u comm : %s\n", current->pid, current->comm);
	}
	//spin_unlock(&lock);
}

void test(struct work_struct *data)								//下半部处理中断;
{
	int i;
	for(i = 0; i < 3; i++)
	{
		msleep(1000);
		//mdelay(1000);
		printk("test: pid : %u comm : %s\n", current->pid, current->comm);
	}
}

struct work_struct work;
irqreturn_t key0_handler(int num, void *dev_id)		//响应中断; 上半部;
{
	int ret;
	struct test_dev *tdev = dev_id;
	printk("test key0 handler num = %d\n", num);
	printk("tdev->name = %s\n", tdev->name);
	
	//INIT_WORK(&work, func);	
	
	ret = schedule_work(&work);
	printk("ret = %d\n", ret);
	//	printk("work in queue\n");

	return IRQ_HANDLED; 
}

struct work_struct work1;
irqreturn_t key1_handler(int num, void *dev_id)		//响应中断; 上半部;
{
	struct test_dev *tdev = dev_id;
	printk("test key1 handler num = %d\n", num);
	printk("tdev->name = %s\n", tdev->name);
	
	//struct work_struct *work = kmalloc();

	INIT_WORK(&work1, test);

	if(schedule_work(&work1) == 0)
		printk("work1 in queue\n");

	return IRQ_HANDLED; 
}

int test_open (struct inode *inode, struct file *filp)
{
	printk("test open\n");	
	func(NULL);
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

	INIT_WORK(&work, func);	

	return 0;
ERROR_register:
	free_irq(IRQ_EINT(27), &tdev);
ERROR_irq1:
	free_irq(IRQ_EINT(26), &tdev);
ERROR_irq0:
	return ret;
}

void test_exit(void)
{
	printk("cleanup module\n");
	unregister_chrdev(tdev.major, DEV_NAME); 
	free_irq(IRQ_EINT(27), &tdev);
	free_irq(IRQ_EINT(26), &tdev);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

