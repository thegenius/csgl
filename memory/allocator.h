#ifndef __ALLOCATOR_H__
#define __ALLOCATOR_H__
#include <stddef.h>

typedef struct allocator* allocator_t;

extern int allocator_create(allocator_t *allocator, size_t num, size_t size);
extern int allocator_delete(allocator_t *allocator);

extern void* allocator_get_memory(allocator_t allocator);
extern void  allocator_put_memory(allocator_t allocator, void* ptr);

#endif
