#ifndef __ALLOCATOR_STRUCT_H__
#define __ALLOCATOR_STRUCT_H__
#include "ring.h"

struct allocator {
    size_t num;
    size_t size;
    void *base;
    ring_t ring;
};

#endif

