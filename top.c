#include <linux/module.h> /* for MODULE_* macros */
#include "connect.h"

static int __init link_init(void)
{
	pr_info("in link_init\n");
	return cpp_init();
}


static void __exit link_exit(void)
{
	cpp_exit();
	pr_info("in link_exit\n");
}

module_init(link_init);
module_exit(link_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module written in C++");
