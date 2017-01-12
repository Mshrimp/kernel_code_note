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
	struct kmem_cache *kmem;
	int *p = NULL;
	int i;
	printk("module init\n");
	kmem = kmem_cache_create("my-kmem-cache", 64, 0, SLAB_HWCACHE_ALIGN, NULL);
	if(IS_ERR_OR_NULL(kmem))
	{
		return -ENOMEM;
	}

	p = kmem_cache_alloc(kmem, GFP_KERNEL);
	if(IS_ERR_OR_NULL(p))
	{
		kmem_cache_destroy(kmem);
		return -ENOMEM;
	}
	printk("p = %p\n", p);	
	for(i = 0; i < 16; i++)
	{
		p[i] = i;
		printk("p[%d] = %d\n", i, p[i]);
	}

	kmem_cache_free(kmem, p);
	kmem_cache_destroy(kmem);
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

