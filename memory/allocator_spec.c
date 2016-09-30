#include <stdio.h>
#include <assert.h>
#include "allocator.h"
#include "allocator_struct.h"


int verify_allocator() {
    const int num = 10;
    const int size = 8;
    allocator_t allocator;
    allocator_create(&allocator, num, size);
    assert(allocator->num == num);
    assert(allocator->size == size);
    assert(allocator->base != 0);
    assert(allocator->ring != 0);

    int *ptr[num];
    for (int i=0; i<num; ++i) {
        ptr[i] = allocator_get_memory(allocator);
        *(ptr[i]) = i;
    }
    assert(allocator_get_memory(allocator) == 0);

    for (int i=0; i<num; ++i) {
        printf("%d\n", *(ptr[i]));
    }


    allocator_put_memory(allocator, ptr[1]);
    allocator_put_memory(allocator, ptr[5]);
    allocator_put_memory(allocator, ptr[2]);
    allocator_put_memory(allocator, ptr[3]);
    assert(allocator_get_memory(allocator) == ptr[1]);
    assert(allocator_get_memory(allocator) == ptr[5]);
    assert(allocator_get_memory(allocator) == ptr[2]);
    assert(allocator_get_memory(allocator) == ptr[3]);



    allocator_delete(&allocator);

    return 1;
}

int main() {

    assert(verify_allocator());


    return 0;
}
