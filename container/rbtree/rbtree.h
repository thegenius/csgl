#ifndef __RBTREE_H__
#define __RBTREE_H__
#include "cdata.h"

typedef struct rbtree rbtree_t;
typedef struct rbtree_node* rbtree_iter_t;
typedef int (*rbtree_cmp_func)(const cdata_t, const cdata_t);

extern int rbtree_create(rbtree_t **tree, rbtree_cmp_func cmp);
extern int rbtree_delete(rbtree_t **tree);

#define rbtree_elem_insert(tree, key, val) __rbtree_elem_insert(tree, (cdata_t)key, (cdata_t)val)
#define rbtree_elem_update(tree, key, val) __rbtree_elem_update(tree, (cdata_t)key, (cdata_t)val)
#define rbtree_elem_delete(tree, idx, val) __rbtree_elem_delete(tree, (cdata_t)idx, (cdata_t*)val)
#define rbtree_elem_remove(tree, key, val) __rbtree_elem_remove(tree, (cdata_t)key, (cdata_t*)val)
#define rbtree_iter_index(tree, idx, iter) __rbtree_iter_index(tree, (cdata_t)idx, iter)
#define rbtree_iter_value(tree, key, iter) __rbtree_iter_value(tree, (cdata_t)key, iter)

extern int __rbtree_elem_insert(rbtree_t *tree, const cdata_t key, const cdata_t val);
extern int __rbtree_elem_update(rbtree_t *tree, const cdata_t key, const cdata_t val);

extern int __rbtree_elem_delete(rbtree_t *tree, const cdata_t idx, cdata_t *val);
extern int __rbtree_elem_remove(rbtree_t *tree, const cdata_t key, cdata_t *val);

extern int __rbtree_iter_index(const rbtree_t *tree, const cdata_t key, rbtree_iter_t *iter);
extern int __rbtree_iter_value(const rbtree_t *tree, const cdata_t val, rbtree_iter_t *iter);

extern int rbtree_iter_head(const rbtree_t *tree, rbtree_iter_t *iter);
extern int rbtree_iter_tail(const rbtree_t *tree, rbtree_iter_t *iter);
extern int rbtree_iter_prev(const rbtree_t *tree, rbtree_iter_t *iter);
extern int rbtree_iter_next(const rbtree_t *tree, rbtree_iter_t *iter);
extern int rbtree_iter_discard(rbtree_t *tree, rbtree_iter_t iter);

#endif
