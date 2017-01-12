#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/io.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/clk.h>
#include <linux/input.h>

#define	DEV_NAME	"test_dev"

struct input_dev *dev;

static void input_dev_init(struct input_dev *dev)
{
	__set_bit(EV_SYN, dev->evbit);	
#if 0
	__set_bit(EV_KEY, dev->evbit);
	__set_bit(KEY_ESC, dev->keybit);
#else
	input_set_capability(dev, EV_KEY, KEY_ESC);
#endif
}

irqreturn_t key_handler(int irq, void *data)
{
	if(gpio_get_value(EXYNOS4_GPX3(2)))
	{
		input_report_key(dev, KEY_ESC, 0);	
	}
	else
	{
		input_report_key(dev, KEY_ESC, 1);
	}
	input_sync(dev);

	return IRQ_HANDLED;
}

static int test_init(void)			
{
	int ret;
	printk("module init\n");
	
	ret = request_irq(IRQ_EINT(26), key_handler, 
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING,
				DEV_NAME, NULL);
	if(IS_ERR_VALUE(ret))
	{
		goto ERROR_irq;
	}

	dev = input_allocate_device();
	if(IS_ERR_OR_NULL(dev))
	{
		ret = -ENOMEM;
		goto ERROR_input_alloc;
	}

	input_dev_init(dev);

	ret = input_register_device(dev);
	if(IS_ERR_VALUE(ret))
	{
		goto ERROR_input_register;
	}

	return 0;
ERROR_input_register:
	input_free_device(dev);
ERROR_input_alloc:
	free_irq(IRQ_EINT(26), NULL);	
ERROR_irq:
	return ret;
}

static void test_exit(void)
{
	printk("cleanup module\n");
	input_unregister_device(dev);
	input_free_device(dev);
	free_irq(IRQ_EINT(26), NULL);
}

module_init(test_init);	
module_exit(test_exit);
MODULE_LICENSE("GPL");

