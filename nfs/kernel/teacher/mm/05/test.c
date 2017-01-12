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
	struct page *pg;
	int *p = NULL;
	int i;
	printk("module init\n");

	//pg = alloc_page(GFP_KERNEL);
	pg = alloc_pages(GFP_KERNEL, get_order(1024));
	if(IS_ERR_OR_NULL(pg))
		return -ENOMEM;

	p = page_address(pg);
	printk("p = %p\n", p);
	printk("PAGE_SIZE = %lx\n", PAGE_SIZE);

	for(i = 0; i < 10; i++)
	{
		p[i] = i;
		printk("p[%d] = %d\n", i, p[i]);
	}
	
	//free_page((unsigned long)p);
	free_pages((unsigned long)p, get_order(1024));
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

