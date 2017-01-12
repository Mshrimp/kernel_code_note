#include <linux/module.h>

int print(const char *str)
{
	return printk(str);
}
EXPORT_SYMBOL(print);

MODULE_LICENSE("GPL");
