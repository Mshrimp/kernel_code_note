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
#include <linux/pwm.h>

#include <plat/gpio-cfg.h>

#define	DEV_NAME	"test_dev"

struct completion com;
struct pwm_device *pwm;
u32 code = 0;
int value = 0;
int offset = 31;
bool end = false;
int test_open (struct inode *inode, struct file *filp)
{
	printk("test open\n");
	return 0;
}

int test_release (struct inode *inode, struct file *filp)
{
	printk("test release \n");
	return 0;
}

ssize_t test_read (struct file *filp, char __user *buf, size_t size, loff_t *offset)
{
	int ret;
	char dev_buf[20];

	if(!end)
	{
		if(wait_for_completion_interruptible(&com))
			return -ERESTARTSYS;
	}

	sprintf(dev_buf, "%#x ", code);
	ret = copy_to_user(buf, dev_buf, strlen(dev_buf));

	code = 0;
	end = false;
		
	return size - ret;
}

struct file_operations fops = {
		.owner = THIS_MODULE,
		.open = test_open,
		.release = test_release,
		.read = test_read,
//		.unlocked_ioctl = test_ioctl,
};

struct miscdevice mdev = {
		.name = DEV_NAME,
		.minor = MISC_DYNAMIC_MINOR,
		.fops = &fops,
};

irqreturn_t pwm0_handler(int irqnum, void *data)
{
	if(value > 200)
	{
		value = 0;
#if 1
		//printf("\n");
		//printf("code = %p\n", code);
		//code = 0;
		offset = 31;
		end = true;
		complete(&com);
#endif
		pwm_disable(pwm);
	}
	else
	{
		value ++;			//100us ++
	}

	return IRQ_HANDLED;
}

irqreturn_t eint_handler(int irqnum, void *data)
{
	if(gpio_get_value(EXYNOS4_GPX1(7)) == 0)
	{
		pwm_disable(pwm);
		if(value >= 7 && value <= 13)
		{
			code &= ~(1<<offset);
			offset --;
		}
		if(value >= 16 && value <= 25)
		{
			code |= (1<<offset);
			offset --;
		}
		value = 0;
		pwm_enable(pwm);
	}

	return IRQ_HANDLED;
}

int test_init(void)			
{
	int ret;
	printk("module init\n");

	pwm = pwm_request(0, DEV_NAME);
	if(IS_ERR_OR_NULL(pwm))
	{
		printk("pwm request\n");
		ret = PTR_ERR(pwm);
		goto ERROR_pwm_request;
	}

	ret = request_irq(IRQ_TIMER0, pwm0_handler, 
				IRQF_TRIGGER_NONE, "timer0_irq", NULL);
	if(IS_ERR_VALUE(ret))
	{
		goto ERROR_irq_timer0;
	}

	ret = request_irq(IRQ_EINT(15), eint_handler, 
				IRQF_TRIGGER_FALLING | IRQF_TRIGGER_RISING, "eint15_irq", NULL);
	if(IS_ERR_VALUE(ret))
	{
		goto ERROR_irq_eint;
	}

	ret = misc_register(&mdev);
	if(IS_ERR_VALUE(ret))
	{
		goto ERROR_misc;
	}

	init_completion(&com);
	pwm_config(pwm, 1, 100000);

	return 0;

ERROR_misc:
	free_irq(IRQ_EINT(15), NULL);
ERROR_irq_eint:
	free_irq(IRQ_TIMER0, NULL);
ERROR_irq_timer0:
	pwm_free(pwm);
ERROR_pwm_request:
	return ret;
}

void test_exit(void)
{
	printk("cleanup module\n");
	misc_deregister(&mdev);
	free_irq(IRQ_EINT(15), NULL);
	free_irq(IRQ_TIMER0, NULL);
	pwm_free(pwm);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

