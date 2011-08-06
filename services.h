#ifndef __services_h
#define __services_h

/**
This file provides headers for all functions and globals provided by the top half. Sometimes the functions declared by this file are actually implemented in the top half and sometimes this is just to declare a kernel function that we wish to use. We cannot include the kernel headers files in C++ code since they confuse the hell out of the C++ compiler and so we resort to declaring these functions here.
*/

typedef int (*func_int_pvoid)(void*);
typedef void (*func_void_pvoid)(void*);
typedef unsigned char (*func_unsigned_char_pvoid_pvoid)(void*,void*);
typedef void (*func_void_pvoid_pvoid_pvoid_pvoid)(void*,void*,void*,void*);

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// infrastructure calls
void capi_init(void);
void capi_finish(void);

// general module name API's
const char* capi_modname(void);
void capi_bug(void);
void capi_stacktrace(void);

// printing and debugging
int capi_printk(const char* fmt,...) __attribute__((format (printf, 1, 2)));
int capi_vprintk(const char* fmt,__builtin_va_list list);
int capi_puts(const char* s);

// These are the pair of function you should use to register/unregister your
// device with the linux kernel
void capi_register_class(void);
void capi_destroy_class(void);
//void* capi_register_dev(callback c,void* data,const char* name) __attribute__((warn_unused_result));
void capi_unregister_dev(void* opaque);

// dma API
void* capi_dma_alloc_coherent(unsigned int* device_pointer,unsigned int size) __attribute__((warn_unused_result));
void capi_dma_free_coherent(void* vaddr,unsigned int device_address,unsigned int size);

// user interaction APIs
int capi_copy_from_user(void* to,const void* from,unsigned int size) __attribute__((warn_unused_result));
int capi_copy_to_user(void* to,const void* from,unsigned int size) __attribute__((warn_unused_result));
int capi_in_passive(void) __attribute__((warn_unused_result));
int capi_in_interrupt(void) __attribute__((warn_unused_result));
int capi_in_irq(void) __attribute__((warn_unused_result));

// memory allocation APIs
void capi_memcpy(const void* to,const void* from,unsigned int size);
void* capi_malloc(unsigned int) __attribute__((warn_unused_result));
void capi_free(void* pointer);
void capi_mdebug(void);

// string functions
void capi_strncpy(char* dst,const char* src,unsigned int size);
char* capi_strcpy(char* dst,const char* src);
int capi_snprintf(char* buf,unsigned int size,const char* fmt,...) __attribute__((format (printf, 3, 4)));
int capi_sprintf(char* dst,const char* fmt,...) __attribute__((format (printf, 2, 3)));
int capi_vsnprintf(char* buf,unsigned int size,const char* fmt,__builtin_va_list list);
int capi_vsprintf(char* buf,const char* fmt,__builtin_va_list list);
int capi_strlen(const char* str);
int capi_strncmp(const char *s1, const char *s2, unsigned int size);
char *capi_strcat(char *dest, const char *src);

// timing functions
unsigned long long capi_get_cycles(void) __attribute__((warn_unused_result));
unsigned long long capi_get_freq(void) __attribute__((warn_unused_result));
void capi_msleep(unsigned long mili);
void capi_udelay(unsigned long micro);
unsigned long capi_get_mic_diff(unsigned long long before,unsigned long long after);

// pci function
void* capi_pci_get_dev(void) __attribute__((warn_unused_result));
int capi_pci_register(func_int_pvoid probe,func_void_pvoid remove);
void capi_pci_unregister(void);
void capi_pci_set_user_data(void* vpci_dev,void* data);
void* capi_pci_get_user_data(void* vpci_dev);
int capi_pci_enable_and_request(void* vpci_dev);
void capi_pci_release_and_disable(void* vpci_dev);
unsigned long capi_pci_resource_start(void* vpci_dev,const int bar);
unsigned long capi_pci_resource_len(void* vpci_dev,const int bar);
void* capi_ioremap(unsigned long start,unsigned long len) __attribute__((warn_unused_result));
void capi_iounmap(void* pointer);
void capi_pci_read_header(void* header,unsigned int offset,unsigned int count,void* vpci_dev);
void capi_pci_write_header(void* header,unsigned int offset,unsigned int count,void* vpci_dev);
unsigned int capi_pci_get_irqnum(void* vpci_dev);
int capi_pci_init(void) __attribute__((warn_unused_result));
int capi_pci_get_msix_irq_num(void);

// spin lock API
void* capi_spinlock_init(void) __attribute__((warn_unused_result));
void capi_spinlock_lock(void* lock);
void capi_spinlock_unlock(void* lock);
void capi_spinlock_free(void* lock);
void capi_spinlock_irq_lock(void* lock);
void capi_spinlock_irq_unlock(void* lock);

// mempool API
void* capi_mempool_create(const char* name,unsigned int size,unsigned int warmup) __attribute__((warn_unused_result));
void* capi_mempool_alloc(void* p) __attribute__((warn_unused_result));
void capi_mempool_free(void* p,void* obj);
void capi_mempool_destroy(void* p);

// IRQ API

typedef func_unsigned_char_pvoid_pvoid user_irq_handler;
void* capi_register_irq(unsigned int irqnum,user_irq_handler irq_handler,void* context) __attribute__((warn_unused_result));
void capi_free_irq(void* handle);
void capi_irq_disable(unsigned int irq);
void capi_irq_enable(unsigned int irq);

// Memory addressing issues
unsigned int capi_get_page_size(void) __attribute__((warn_unused_result));
unsigned long capi_virt_to_phys(void* virt) __attribute__((warn_unused_result));
void capi_zeromem(void* addr,unsigned int size);
void capi_memset(void* addr,int c,unsigned int size);

// IO operations
unsigned char capi_inb(unsigned int port);
unsigned short capi_inw(unsigned int port);
unsigned int capi_inl(unsigned int port);
void capi_insb(unsigned int port,void* addr,unsigned long count);
void capi_insw(unsigned int port,void* addr,unsigned long count);
void capi_insl(unsigned int port,void* addr,unsigned long count);
void capi_outb(unsigned char val,unsigned port);
void capi_outw(unsigned short val,unsigned port);
void capi_outl(unsigned int val,unsigned port);
void capi_outsb(unsigned int port,void* addr,unsigned long count);
void capi_outsw(unsigned int port,void* addr,unsigned long count);
void capi_outsl(unsigned int port,void* addr,unsigned long count);
unsigned char capi_ioread8(void* addr);
unsigned short capi_ioread16(void* addr);
unsigned int capi_ioread32(void* addr);
void capi_iowrite8(unsigned char data,void* addr);
void capi_iowrite16(unsigned short data,void* addr);
void capi_iowrite32(unsigned int data,void* addr);
void capi_ioread8_rep(void* addr,void* buf,unsigned long count);
void capi_ioread16_rep(void* addr,void* buf,unsigned long count);
void capi_ioread32_rep(void* addr,void* buf,unsigned long count);
void capi_iowrite8_rep(void* addr,void* buf,unsigned long count);
void capi_iowrite16_rep(void* addr,void* buf,unsigned long count);
void capi_iowrite32_rep(void* addr,void* buf,unsigned long count);
void capi_smp_mb(void);

// CACHE functions
void* capi_cache_create(const char* name,unsigned int size,unsigned int warmup) __attribute__((warn_unused_result));
void* capi_cache_alloc(void* cache) __attribute__((warn_unused_result));
void capi_cache_free(void* cache,void* obj);
void capi_cache_destroy(void* cache);

//User memory allocation functions
void* capi_user_to_kernel_adr(void* user) __attribute__((warn_unused_result));
//int capi_put_vma_data(struct vm_area_struct* vma,void* adr,mem_type type) __attribute__((warn_unused_result));
void* capi_get_vma_adr(struct vm_area_struct* vma) __attribute__((warn_unused_result));
int capi_is_our_vma(struct vm_area_struct* vma) __attribute__((warn_unused_result));
int capi_is_our_uadr(unsigned int uadr) __attribute__((warn_unused_result));
int capi_free_vma(struct vm_area_struct* vma) __attribute__((warn_unused_result));
unsigned int capi_round_to_page(unsigned int size);
void* capi_alloc_user_mem(unsigned int size) __attribute__((warn_unused_result));
void capi_free_user_mem(void* kptr,unsigned int size);

// Tasklet stuff
typedef func_void_pvoid_pvoid_pvoid_pvoid user_tasklet_handler;
void* capi_tasklet_init(user_tasklet_handler func) __attribute__((warn_unused_result));
void capi_tasklet_schedule(void* handle,void* param1,void* param2,void* param3,void* param4);
void capi_tasklet_free(void* handle);
void capi_tasklet_disable(void);
void capi_tasklet_enable(void);

// List stuff
void* capi_list_create(void);
void capi_list_destroy(void* ptr);
int capi_list_isempty(void* lptr);
void* capi_list_iter_init(void* lptr);
int capi_list_iter_isover(void* lptr,void* iter);
void* capi_list_iter_getval(void* lptr,void* iter);
void* capi_list_iter_next(void* lptr,void* iter);
void capi_list_iter_del(void* lptr,void* iter);
void* capi_list_pop(void* lptr);
void capi_list_add(void* lptr,void* ptr);

// map to user stuff
unsigned long capi_map_to_user(void* filp,void* kptr,unsigned int size,bool use_remap,bool ro,bool reserve,unsigned int type) __attribute__((warn_unused_result));
int capi_unmap(unsigned long uadr,bool unreserve,void** pkadr,unsigned int* psize,bool showMercy) __attribute__((warn_unused_result));
int kern_mmap(struct file *filp, struct vm_area_struct *vma);
void capi_print_page(struct page *page);
void capi_buffer_reserve(void* kadr,unsigned int size);
void capi_buffer_unreserve(void* kadr,unsigned int size);
void capi_buffer_print(void* kadr,unsigned int size);
void capi_buffer_use(void* kadr,unsigned int size);
void capi_buffer_unuse(void* kadr,unsigned int size);
void* capi_uadr_to_kadr(unsigned int uadr) __attribute__((warn_unused_result));

// get user pages
int capi_get_user_pages(void* b_pointer,unsigned int b_size,void** pptr,void** pvptr,unsigned int* pnr_pages,void** ppages) __attribute__((warn_unused_result));
void capi_release_pages(void* vptr,void* ppages,unsigned int nr_pages);

// events
int capi_events_drv_init(void) __attribute__((warn_unused_result));
void capi_events_drv_fini(void);
int capi_events_create(void) __attribute__((warn_unused_result));
int capi_events_set(unsigned int num) __attribute__((warn_unused_result));
int capi_events_reset(unsigned int num) __attribute__((warn_unused_result));
//int capi_events_wait_forever(WFSO* wfso) __attribute__((warn_unused_result));
//int capi_events_wait_timeout(WFSO* wfso) __attribute__((warn_unused_result));
int capi_events_close(unsigned int num) __attribute__((warn_unused_result));

// file data
void capi_filedata_setup(struct file* filp);
void capi_filedata_teardown(struct file* filp);

// completion stuff
void* capi_completion_malloc(void) __attribute__((warn_unused_result));
void capi_completion_free(void* vcomp);
void capi_completion_init(void* vcomp);
void capi_completion_wait_forever_interruptible(void* vcomp);
unsigned long capi_completion_wait_timeout_interruptible(void* vcomp,unsigned int msecs) __attribute__((warn_unused_result));
void capi_completion_wait_forever(void* vcomp);
void capi_completion_complete_all(void* vcomp);

// registry stuff
int capi_registry_entry(const char* key);
void capi_registry_set(const char* key,const char* val);
int capi_registry_read(void);
int capi_registry_init(void) __attribute__((warn_unused_result));
int capi_registry_fini(void) __attribute__((warn_unused_result));
int capi_registry_write(void) __attribute__((warn_unused_result));
int capi_registry_flush(void) __attribute__((warn_unused_result));
char* capi_registry_get(const char* key);

char* capi_registry_get_string(const char* key,char* defval) __attribute__((warn_unused_result));
int capi_registry_get_bool(const char* key,int defval) __attribute__((warn_unused_result));
int capi_registry_get_int(const char* key,int defval) __attribute__((warn_unused_result));
unsigned int capi_registry_get_uint(const char* key,unsigned int defval) __attribute__((warn_unused_result));

// atoi atuff
int capi_atoi(const char *name) __attribute__((warn_unused_result));
unsigned int capi_atou(const char *name) __attribute__((warn_unused_result));

// dmam stuff
int capi_dmam_init(void) __attribute__((warn_unused_result));
int capi_dmam_fini(void) __attribute__((warn_unused_result));
void* capi_dmam_alloc_coherent(unsigned int* device_pointer,unsigned int size) __attribute__((warn_unused_result));
void capi_dmam_free_coherent(void* vaddr,unsigned int device_address,unsigned int size);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __services_h
