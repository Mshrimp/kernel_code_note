#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>

#define	DEV_NAME	"func_dev"

#define KEY0	EXYNOS4_GPX3(2)

struct func_dev{
	char *name;
};

irqreturn_t key0_handler(int num, void *dev_id)
{
	struct func_dev *tdev = dev_id;
	printk("func irq handler num = %d\n", num);
	printk("tdev->name = %s\n", tdev->name);
	return IRQ_HANDLED; 
}

struct func_dev tdev = {
	.name = "funckey0",
};

int func_init(void)			
{
	int ret;
	printk("module init\n");
	ret = request_irq(gpio_to_irq(KEY0), key0_handler, 
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING | IRQF_SHARED, 
				"func-key0", &tdev);
//	ret = request_irq(IRQ_EINT(26), irq_handler, 
//				IRQF_TRIGGER_FALLING, "func-key0", NULL);
	if(IS_ERR_VALUE(ret))
	{
		return ret;
	}
	return 0;
}

void func_exit(void)
{
	printk("cleanup module\n");
	free_irq(IRQ_EINT(26), &tdev);
}

module_init(func_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(func_exit);
MODULE_LICENSE("GPL");

