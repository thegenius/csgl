#ifndef __RBTREE_H__
#define __RBTREE_H__
#include "cdata.h"

typedef struct rbtree rbtree_t;
typedef struct rbtree_iter rbtree_iter_t;

extern int rbtree_create(rbtree_t **tree);
extern int rbtree_delete(rbtree_t **tree);

extern int rbtree_elem_insert(rbtree_t *tree, const cdata_t key, const cdata_t val);
extern int rbtree_elem_update(rbtree_t *tree, const cdata_t key, const cdata_t val);

extern int rbtree_elem_delete(rbtree_t *tree, const cdata_t idx, cdata_t *val);
extern int rbtree_elem_remove(rbtree_t *tree, const cdata_t key, cdata_t *val);

extern int rbtree_iter_index(const rbtree_t *tree, const cdata_t key, rbtree_iter_t *iter);
extern int rbtree_iter_value(const rbtree_t *tree, const cdata_t val, rbtree_iter_t *iter);

extern int rbtree_iter_head(const rbtree_t *tree, rbtree_iter_t *iter);
extern int rbtree_iter_tail(const rbtree_t *tree, rbtree_iter_t *iter);
extern int rbtree_iter_prev(const rbtree_t *tree, rbtree_iter_t *iter);
extern int rbtree_iter_next(const rbtree_t *tree, rbtree_iter_t *iter);

extern int rbtree_check_internal();


#endif
