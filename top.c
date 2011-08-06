#include <linux/module.h> // for MODULE_* macros

int cpp_init(void);
void cpp_exit(void);

static int __init link_init(void) {
	return(cpp_init());
}


static void __exit link_exit(void) {
	cpp_exit();
}

module_init(link_init);
module_exit(link_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Veltzer");
MODULE_DESCRIPTION("Module written in C++");

