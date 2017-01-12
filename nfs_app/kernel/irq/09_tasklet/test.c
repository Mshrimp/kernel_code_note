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
void func(unsigned long data)						//下半部处理中断;
{
	int i;
	local_bh_disable();
	//preempt_count() += 512;							//禁止软中断;
	//preempt_disable();	preempt_count() ++;		//禁止线程抢占;
	spin_lock(&lock);
	for(i = 0; i < 3; i++)
	{
		//msleep(1000);
		mdelay(2000);
		printk("func: [%s] pid : %u comm : %s\n", (char *)data, current->pid, current->comm);
	}
	spin_unlock(&lock);
	//preempt_enable();
	//preempt_count() -= 512;
	local_bh_enable();
}

struct tasklet_struct tl;
irqreturn_t key0_handler(int num, void *dev_id)		//响应中断; 上半部;
{
	struct test_dev *tdev = dev_id;
	printk("test key0 handler num = %d\n", num);
	printk("tdev->name = %s\n", tdev->name);
	
	tasklet_schedule(&tl);

	return IRQ_HANDLED; 
}

int test_open (struct inode *inode, struct file *filp)
{
	printk("test open\n");	
	func((unsigned long)"test open");
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

	ret = register_chrdev(tdev.major, DEV_NAME, &tdev.fops);
	if(IS_ERR_VALUE(ret))
		goto ERROR_register;

	if(tdev.major == 0)
	{
		tdev.major = ret;
	}
	printk("major : %d \n", tdev.major);

	spin_lock_init(&lock);

	tasklet_init(&tl, func, (unsigned long)"key0");

	return 0;
ERROR_register:
	free_irq(IRQ_EINT(26), &tdev);
ERROR_irq0:
	return ret;
}

void test_exit(void)
{
	printk("cleanup module\n");
	unregister_chrdev(tdev.major, DEV_NAME); 
	free_irq(IRQ_EINT(26), &tdev);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

