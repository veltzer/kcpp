#include "services.h"
/* to satisfy the kernel dynamic linker */
int __gxx_personality_v0;
int _Unwind_Resume;

/* support code for new and delete */
void *operator new(unsigned int x) {
	return(capi_malloc(x));
}


void operator delete(void *pointer) {
	capi_free(pointer);
}

int cpp_init() {
	a = new A();

	return(0);
}

void cpp_exit() {
	delete a;
}
