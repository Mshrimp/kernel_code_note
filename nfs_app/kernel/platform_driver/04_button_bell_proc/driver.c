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
#include <linux/proc_fs.h>

#define PROC_NAME	"keystat"
#define	DEV_NAME	"test_dev"

unsigned long gpio[4] = {EXYNOS4_GPX3(2), EXYNOS4_GPX3(3), EXYNOS4_GPX3(4), EXYNOS4_GPX3(5)};
unsigned int irq[4] = {IRQ_EINT(26), IRQ_EINT(27), IRQ_EINT(28), IRQ_EINT(29)};
char buf[4] = {};
int ev_down = 0;
wait_queue_head_t wq;

int key_read_proc(char *page, char **start, off_t off, 
						int count, int *eof, void *data)
{
	int ret;
	
	/*
	if(!ev_down)
	{
		wait_event_interruptible(wq, ev_down);
	}
	ev_down = 0;
	*/

	ret = sprintf(page, "key: %c %c %c %c\n", buf[0], buf[1], buf[2], buf[3]);
	return ret;		
}

int key_write_proc(struct file *file, const char __user *buffer,
       					unsigned long count, void *data)
{
	if(!strncmp(buffer, "on", 2))
	{
		printk("bell on\n");
		gpio_direction_output(EXYNOS4_GPD0(0), 1);
	}
	else if(!strncmp(buffer, "off", 3))
	{
		printk("bell off\n");
		gpio_direction_output(EXYNOS4_GPD0(0), 0);
	}
	return count;
}

irqreturn_t irq_handler(int irq, void *data)
{
	int i;
	for(i = 0; i < 4; i++)
	{
		buf[i] = '0' + !gpio_get_value(gpio[i]);
		//buf[i] = gpio_direction_input(irq_to_gpio(irq[i]));
	}
	ev_down = 1;
	//wake_up_interruptible(&wq);
	return IRQ_HANDLED;
}

struct proc_dir_entry *entry;
int test_init(void)			
{
	int ret;
	int i;
	printk("module init\n");
	for(i = 0; i < 4; i++)
	{
		ret = request_irq(irq[i], irq_handler, 
				 IRQF_TRIGGER_FALLING, DEV_NAME, NULL);
		if(IS_ERR_VALUE(ret))
		{
			for(--i; i>=0; i--)
			{
				free_irq(irq[i], NULL);
			}
			return -EBUSY;
		}
	}
	entry = create_proc_entry(PROC_NAME, 0777, NULL);
	if(IS_ERR_OR_NULL(entry))
		return -ENOMEM;

	entry->read_proc = key_read_proc;
	entry->write_proc = key_write_proc;

	init_waitqueue_head(&wq);
	return 0;
}

void test_exit(void)
{
	int i;
	printk("cleanup module\n");
	//misc_deregister(&tdev.mdev);
	remove_proc_entry(PROC_NAME, NULL);
	for(i = 0; i < 4; i++)
		free_irq(irq[i], NULL);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

