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
#if 1
	__set_bit(EV_KEY, dev->evbit);
	__set_bit(BTN_TOUCH, dev->keybit);

	__set_bit(EV_ABS, dev->evbit);
	__set_bit(ABS_X, dev->absbit);
	__set_bit(ABS_Y, dev->absbit);
	__set_bit(ABS_PRESSURE, dev->absbit);
#else
	input_set_capability(dev, EV_KEY, BTN_TOUCH);
	input_set_capability(dev, EV_ABS, ABS_X);
	input_set_capability(dev, EV_ABS, ABS_Y);
	input_set_capability(dev, EV_ABS, ABS_PRESSURE);
#endif

	input_set_abs_params(dev, ABS_X, 0, 800, 0, 0);
	input_set_abs_params(dev, ABS_Y, 0, 480, 0, 0);
	input_set_abs_params(dev, ABS_PRESSURE, 0, 255, 0, 0);
}

int x = 10;
int y = 10;
irqreturn_t key_handler(int irq, void *data)
{
	if(gpio_get_value(EXYNOS4_GPX3(2)))
	{
		input_report_key(dev, BTN_TOUCH, 0);	
		input_report_abs(dev, ABS_PRESSURE, 0);
	}
	else
	{
		input_report_key(dev, BTN_TOUCH, 1);
		input_report_abs(dev, ABS_X, x);
		input_report_abs(dev, ABS_Y, y);
		input_report_abs(dev, ABS_PRESSURE, 200);
		x += 10;
		y += 10;
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

