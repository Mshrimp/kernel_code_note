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

#define	DEV_NAME	"test_dev"

int test_init(void)			
{
	printk("module init\n");
	printk("VMALLOC_END = %lx\n", VMALLOC_END);
	printk("VMALLOC_START = %lx\n", VMALLOC_START);
	printk("high_memory = %p\n", high_memory);
	printk("KMALLOC_MAX_SIZE = %lx\n", KMALLOC_MAX_SIZE);
	printk("PAGE_OFFSET = %lx\n", PAGE_OFFSET);
	printk("MODULES_VADDR = %lx\n", MODULES_VADDR);
	printk("TASK_SIZE = %lx\n", TASK_SIZE);

	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

