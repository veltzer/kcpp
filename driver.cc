#include "services.h"

class Driver {
public:
	int x;
	Driver() {
		service_printk("inside constructor!\n");
	}
	~Driver() {
		service_printk("inside destructor!\n");
	}
};

static Driver* driver;

extern "C" {
	int cpp_init(void);
	void cpp_exit(void);
}

void test_kmalloc(void) {
	void* p = service_malloc(1000);
	service_free(p);
}

int cpp_init() {
	test_kmalloc();

	driver = new Driver();

	return(0);
}

void cpp_exit() {
	delete driver;
}
