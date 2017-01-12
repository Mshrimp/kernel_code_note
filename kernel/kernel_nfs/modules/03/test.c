#include <linux/module.h>

int print(const char *);
int init_module(void)
{
	print("hello test kernel\n");
	return 0;
}

void cleanup_module(void)
{
	print("cleanup module\n");
}

MODULE_LICENSE("GPL");

