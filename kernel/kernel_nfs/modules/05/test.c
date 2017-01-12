#include <linux/module.h>

int print(const char *);
int init_module(void)
{
	#ifdef	MODULE
		print("hello test module\n");
	#else
		print("hello test kernel\n");
	#endif
	return 0;
}

void cleanup_module(void)
{
	print("cleanup module\n");
}

MODULE_LICENSE("GPL");

