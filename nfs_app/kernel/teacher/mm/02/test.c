#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>

#define	DEV_NAME	"test_dev"

int test_init(void)			
{
	int *p = NULL;
	int i;
	printk("module init\n");
	p = vmalloc(40);
	if(IS_ERR_OR_NULL(p))
		return -ENOMEM;

	printk("p = %p\n", p);
	for(i = 0; i < 10; i++)
	{
		p[i] = i;
		printk("p[%d] = %d\n", i, p[i]);
	}
	
	vfree(p);
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

