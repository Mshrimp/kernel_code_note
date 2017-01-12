#include <linux/module.h>

int print(const char *);

int func_init(void)
//int init_module(void)
{
	printk("hello test kernel!\n");
	return 0;
}

void func_exit(void)
//void cleanup_module(void)
{
	printk("cleanup module!\n");
}

module_init(func_init);
module_exit(func_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("xiami  xiamizzu@163.com");
MODULE_VERSION("1.0");



