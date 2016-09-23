#include <stdio.h>
#include "bsearch.h"
int binary_search(const void *arr, const size_t len, const size_t size, const void *target, binary_search_cmp_func cmp) {
    int head = -1;
    int tail = len;
    while (head + 1 < tail) {
        int mid = head + (int)((unsigned)(tail - head) >>1);
        if (cmp(target, arr + (mid*size) ) > 0) {
            head = mid;
        } else {
            tail = mid;
        }
    }
    if (tail == len || cmp(target, arr + (tail*size)) != 0) {
        /* we can not return tail, because tail may be 0 and target not found.
           let ret = -tail-1, we can use -ret-1 to get the correct index */
        return -tail-1;     
    } else {
        return tail;
    }
}
