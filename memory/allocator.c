#include <stdio.h>
#include <stdlib.h>

#include "ring.h"
#include "allocator.h"
#include "allocator_struct.h"



int allocator_create(allocator_t *allocator, size_t num, size_t size) {
    (*allocator) = (struct allocator*)malloc(sizeof(struct allocator));
    if (*allocator == 0) {
        return -1;
    }
    (*allocator)->base = aligned_alloc(size, num * size);
    if ((*allocator)->base == 0) {
        return -1;
    }
    int err = ring_create(&((*allocator)->ring), num);
    if (err) {
        return -1;
    }
    for (int i=0; i<num; ++i) {
        ring_push((*allocator)->ring, i * size);
    }
    (*allocator)->num = num;
    (*allocator)->size = size;
    return 0;
}

int allocator_realloc(allocator_t allocator, size_t num) {
    allocator->base = realloc(allocator->base, 2*allocator->num*allocator->size);
    if (allocator->base == 0) {
        return -1;
    }
    for (int i=0; i<num; ++i) {
        ring_push(allocator->ring, allocator->num + i * allocator->size);
    }
    allocator->num += num;
    return 0;
}

int allocator_delete(allocator_t *allocator) {
    ring_delete(&((*allocator)->ring));
    free((*allocator)->base);
    free(*allocator);
    *allocator = 0;
    return 0;
}

void* allocator_get_memory(allocator_t allocator) {
    if (ring_empty(allocator->ring)) {
        int err = allocator_realloc(allocator, 2*allocator->num);
        if (err) {
            return (void*)0;
        }
    }
    size_t offset;
    ring_pull(allocator->ring, &offset);
    return allocator->base + offset;
}

void allocator_put_memory(allocator_t allocator, void* ptr) {
    size_t offset = ptr - allocator->base;
    if (ring_push(allocator->ring, offset) == -1) {
        perror("ERROR: - allocator_put_memory - : you return ptr more than once!");
        return;
    }
}


