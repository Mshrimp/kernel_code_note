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

#define	DEV_NAME	"test_dev"

int test_init(void)			
{
	unsigned int phys = 0;
	int *virt = NULL;
	int i;
	printk("module init\n");
	virt = dma_alloc_coherent(NULL, 1024, &phys, GFP_KERNEL);
	if(IS_ERR_OR_NULL(virt))
	{
		return -ENOMEM;
	}

	printk("phys = %x\n", phys);
	printk("virt = %p\n", virt);

	for(i = 0; i < 256; i++)
	{
		virt[i] = i;
		printk("virt[%d] = %d\n", i, virt[i]);
	}

	dma_free_coherent(NULL, 1024, virt, phys);
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

