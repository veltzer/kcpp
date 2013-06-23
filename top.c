#include <linux/module.h> /* for MODULE_* macros */
#include <linux/slab.h>	/* for kmalloc */
#include "connect.h"

void *service_malloc2(unsigned int size)
{
	void *p;
	p = kmalloc(size+1, GFP_KERNEL);
	return p;
}

static int __init link_init(void)
{
	char *p = kmalloc(1000, GFP_KERNEL);
	mb();
	kfree(p);
	return cpp_init();
}


static void __exit link_exit(void)
{
	cpp_exit();
}

module_init(link_init);
module_exit(link_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module written in C++");
