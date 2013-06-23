#include "services.h"

/* to satisfy the kernel dynamic linker */
//int __gxx_personality_v0;
//int _Unwind_Resume;
//int _GLOBAL_OFFSET_TABLE_;

/* support code for new and delete */
void *operator new(unsigned int x) {
	return(service_malloc(x));
}


void operator delete(void *pointer) {
	service_free(pointer);
}
