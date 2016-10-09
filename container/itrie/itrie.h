#ifndef __ITRIE_H__
#define __ITRIE_H__
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>
#include "cdata.h"

typedef struct itrie* itrie_t;
typedef struct itrie_node* itrie_node_t;
typedef struct itrie_pair itrie_pair_t;

extern int itrie_create(itrie_t *itrie);
extern int itrie_delete(itrie_t *itrie);
extern size_t itrie_size(itrie_t itrie);
extern int __itrie_insert(itrie_t itrie, const cdata_t key, const cdata_t val);
extern int __itrie_remove(itrie_t itrie, const cdata_t key, cdata_t *val);

#define itrie_insert(itrie, key, val) __itrie_insert(itrie, (cdata_t)key, (cdata_t)(val))
#define itrie_remove(itrie, key, val) __itrie_remove(itrie, (cdata_t)key, (cdata_t*)(val))

#endif
