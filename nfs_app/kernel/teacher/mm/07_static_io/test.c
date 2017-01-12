#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/mm.h>
#include <linux/dma-mapping.h>
#include <linux/io.h>

#include <plat/map-base.h>
#include <plat/map-s5p.h>

#define	DEV_NAME	"test_dev"

int test_init(void)			
{
	void *chipid;
	printk("module init\n");
	chipid = S5P_VA_CHIPID;
	
	//ioread32();
	printk("chipid = %lx\n", *(unsigned long *)chipid);

	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

