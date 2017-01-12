#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

#define	DEV_NAME	"test_dev"

irqreturn_t irq_handler(int num, void *dev_id)
{
	printk("irq handler\n");
	return IRQ_HANDLED; 
}

int test_init(void)			
{
	int ret;
	printk("module init\n");
	ret = request_irq(IRQ_EINT(26), irq_handler, 
				IRQF_TRIGGER_FALLING, "test-key0", NULL);
	if(IS_ERR_VALUE(ret))
	{
		return ret;
	}
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	free_irq(IRQ_EINT(26), NULL);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

