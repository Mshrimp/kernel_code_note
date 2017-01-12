#include <linux/module.h>
#include <linux/fs.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/sched.h>

#define	DEV_NAME	"test_dev"

#define __preempt_disable()		\
			current_thread_info()->preempt_count++	//访问公共时不可抢占;
#define __preempt_enable()		\
			current_thread_info()->preempt_count--	//访问结束可以抢占;

volatile int lock = 0;
void func(void)
{
	int i;
	while(lock == 1)
		;
	lock = 1;
	for(i = 0; i < 3; i++)
	{
		mdelay(1000);
		printk("pid = %d comm = %s cpu = %d\n", 
			current->pid, current->comm, current_thread_info()->cpu);
	}
	lock = 0;
}

int test_open (struct inode *inode, struct file *filp)
{
	int tmp = 0;
	int val;
	int ret = 0;
	printk("test open\n");
	asm __volatile__(
		"ldrex r0, [%1]\n\t"
		"mov %0, r0\n\t"
		: "=r" (val)
		: "r" (&tmp)
		: "r0"
	);
	printk("val = %d\n", val);

	asm __volatile__(
		"ldrex r0, [%1]\n\t"
		"mov r0, #1\n\t"
		"strex %0, r0, [%1]\n\t"
	//	"strex %0, r0, [%1]\n\t"
		: "=r" (ret) 
		: "r" (&tmp)
		: "r0"
	);

	printk("tmp = %d ret = %d\n", tmp, ret);
	

	return 0;
}

int test_release (struct inode *inode, struct file *filp)
{
	printk("test release\n");
	return 0;
}

struct file_operations fops = {
		.owner = THIS_MODULE,		//引用计数; 
		.open = test_open,			//可选实现;
		.release = test_release,	//可选实现;
};

int major = 0;
int test_init(void)			
{
	int ret = 0;
	printk("module init\n");
	
	//1. 创建并注册一个字符设备到内核; cdev
	ret = register_chrdev(major, DEV_NAME, &fops);
	if(ret < 0)
	{
		return ret;
	}
	if(major == 0)		//向内核动态申请; 
	{
		major = ret;	//返回值就是主设备号;
		printk("major = %d\n", major);
	}
	else					//向内核静态申请;
	{
		printk("major = %d\n", major);
	}

	return 0;
}

void test_exit(void)
{
	printk("cleanup module\n");
	unregister_chrdev(major, DEV_NAME);
}

module_init(test_init);	//1)启动时自动调用2)插入模块时调用;
module_exit(test_exit);
MODULE_LICENSE("GPL");

