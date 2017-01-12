#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

#define	DEV_NAME	"test_dev"

#define KEY0	EXYNOS4_GPX3(2)

struct test_dev{
	char *name;

	int major;
	struct file_operations fops;
};

//struct mutex mutex;
spinlock_t lock;
void func(char *str)								//下半部处理中断;
{
	int i;
	unsigned long flags;
	spin_lock_irqsave(&lock, flags);
	//local_irq_save(flags);
	//local_irq_disable();
	//spin_lock_irq();
	//spin_lock(&lock);
	for(i = 0; i < 3; i++)
	{
		//msleep(1000);
		mdelay(1000);
		printk("[%s] pid : %u comm : %s\n", str, current->pid, current->comm);
	}
	//spin_unlock(&lock);
	//local_irq_enable();
	//local_irq_restore(flags);
	//spin_unlock_irq();
	spin_unlock_irqrestore(&lock, flags);
}

irqreturn_t key0_handler(int num, void *dev_id)		//响应中断; 上半部;
{
	struct test_dev *tdev = dev_id;
	printk("test irq handler num = %d\n", num);
	printk("tdev->name = %s\n", tdev->name);

	func("irq");									//处理中断;
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
	ret = request_irq(gpio_to_irq(KEY0), key0_handler, 
				IRQF_TRIGGER_FALLING, 
				"test-key0", &tdev);
//	ret = request_irq(IRQ_EINT(26), irq_handler, 
//				IRQF_TRIGGER_FALLING, "test-key0", NULL);
	if(IS_ERR_VALUE(ret))
		goto ERROR_irq;

	ret = register_chrdev(tdev.major, DEV_NAME, &tdev.fops);
	if(IS_ERR_VALUE(ret))
		goto ERROR_register;

	if(tdev.major == 0)
	{
		tdev.major = ret;
	}
	printk("major : %d \n", tdev.major);

	spin_lock_init(&lock);

	return 0;
ERROR_register:
	free_irq(IRQ_EINT(26), &tdev);
ERROR_irq:
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

