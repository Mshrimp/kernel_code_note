#include <linux/module.h>

int init_module(void)
{
	printk("hello test kernel\n");
	return 0;
}

void cleanup_module(void)
{
	printk("cleanup module\n");
}

