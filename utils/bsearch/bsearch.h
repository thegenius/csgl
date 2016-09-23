#ifndef __BSEARCH_H__
#define __BSEARCH_H__
#include <stddef.h>

/********************************************************************
    return index of the target if find the target, inde:[0, len - 1]
    return negtive index if the target is not found.
    - ret - 1 be the index you can insert to keep the sequence
*********************************************************************/
typedef int (*binary_search_cmp_func)(const void*, const void*);
extern int binary_search(const void *arr, const size_t len, const size_t size, const void *target, binary_search_cmp_func cmp);
static inline int binary_search_insert_pos(const int idx) {
    return idx>0 ? idx : -idx-1; 
}

#endif
