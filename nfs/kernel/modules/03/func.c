#include <linux/module.h>


int print(const char *str)
{
	return printk(str);
}


