#ifndef __ITRIE_H__
#define __ITRIE_H__
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>
#include "cdata.h"
#include "itrie_inline.h"

typedef struct itrie* itrie_t;
typedef struct itrie_pair itrie_pair_t;
typedef struct itrie_node* itrie_node_t;
typedef struct itrie_iter* itrie_iter_t;

extern int itrie_create(itrie_t *itrie);
extern int itrie_delete(itrie_t *itrie);
extern size_t itrie_size(itrie_t itrie);
extern int __itrie_elem_insert(itrie_t itrie, const cdata_t key, const cdata_t val);
extern int __itrie_elem_remove(itrie_t itrie, const cdata_t key, cdata_t *val);


extern int __itrie_iter_search(itrie_t itrie, const cdata_t key, itrie_iter_t iter);
//extern int __itrie_iter_locate(itrie_t itrie, const cdata_t key, itrie_iter_t iter);

#define itrie_elem_insert(itrie, key, val) __itrie_elem_insert(itrie, (cdata_t)key, (cdata_t)(val))
#define itrie_elem_remove(itrie, key, val) __itrie_elem_remove(itrie, (cdata_t)key, (cdata_t*)(val))
#define itrie_iter_search(itrie, key, iter) __itrie_iter_search(itrie, (cdata_t)key, iter)

#endif
