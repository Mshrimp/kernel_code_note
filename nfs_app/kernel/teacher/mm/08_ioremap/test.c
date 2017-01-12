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
#include <linux/io.h>

#include <plat/map-base.h>

#define	DEV_NAME	"test_dev"

#define WTCON		0x10060000

int test_init(void)			
{
	void *chipid;
	void *wdt_reg;
	void *sta_wdt;
	unsigned int phys = 0x10000000;
	printk("module init\n");
	chipid = ioremap(phys, 8);
	if(IS_ERR_OR_NULL(chipid))
		return -ENOMEM;
	
	//ioread32();
	printk("chipid = %lx\n", *(unsigned long *)chipid);
	printk("chipid = %p\n", chipid);

	wdt_reg = ioremap(WTCON, 64);
	if(IS_ERR_OR_NULL(wdt_reg))
	{
		iounmap(chipid);
		return -ENOMEM;
	}

	printk("WTCON = %x\n", ioread32(wdt_reg));
	printk("wdt_reg = %p\n", wdt_reg);

	sta_wdt = S3C_VA_WATCHDOG;
	printk("WTCON = %x\n", *(u32 *)sta_wdt);
	printk("sta_wdt = %p\n", sta_wdt);

	iounmap(chipid);
	iounmap(wdt_reg);
	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

