#ifndef __TREEARRAY_H__
#define __TREEARRAY_H__
#include <stddef.h>
#include <stdint.h>
typedef struct treearray_t treearray_t;

extern size_t treearray_at(treearray_t *arr, size_t i);
extern int treearray_create(treearray_t **arr, const size_t size);
extern int treearray_delete(treearray_t **arr);
extern int treearray_update(treearray_t *arr, const size_t idx, const int val);
extern int treearray_length(const treearray_t *arr, const size_t idx);
extern int treearray_locate(const treearray_t *arr, const size_t loc);

#endif
