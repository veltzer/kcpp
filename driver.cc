#include "services.h"

class Driver {
public:
	int x;
	Driver() {
		capi_printk("inside constructor!\n");
	}
	~Driver() {
		capi_printk("inside destructor!\n");
	}
};
