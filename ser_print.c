#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/interrupt.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/types.h>
#include <linux/proc_fs.h>
#include <linux/mm.h>
#include <linux/timex.h>
#include <linux/cpufreq.h>

#include "services.h"
#include "kernel_helper.h"

/*
	Module name function
*/
const char* capi_modname(void) {
	return myname;
}

/*
	Throw a bug
*/
void capi_bug(void) {
	BUG();
}

/*
	A printing function. This function supports var args. 
*/
int capi_printk(const char* fmt,...) {
	va_list args;
	int ret;
	va_start(args,fmt);
	ret=vprintk(fmt, args);
	va_end(args);
	return ret;
}
/*
	A var arg printk version
*/
int capi_vprintk(const char* fmt,va_list list) {
	return vprintk(fmt, list);
}

/*
	A simple string printing function
*/
int capi_puts(const char* str) {
	return printk("%s",str);
}

/*
	A stack trace service
*/
void capi_stacktrace(void) {
/*
	#ifdef CONFIG_STACKTRACE
	struct stack_trace my_trace;
	save_stack_trace(&my_trace);
	print_stack_trace(&my_trace,0);
	#endif // CONFIG_STACKTRACE
*/
}
