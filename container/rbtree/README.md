# This is a red-black-tree implementation with clear interface and careful optimization.  
1.  use parent pointer to store pointer, index and color.
2.  use head and tail node to simplify the implementation.
3.  add size field to get a log(N) algorithm to find n-th element.

# Data Structure  
```c
typedef struct rbtree rbtree_t;  
typedef struct rbtree_node* rbtree_iter_t;  
typedef int (*rbtree_cmp_func)(const cdata_t, const cdata_t);  
```
# Interface  
```c
extern int rbtree_create(rbtree_t **tree, rbtree_cmp_func cmp);  
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
extern int rbtree_iter_discard(rbtree_t *tree, rbtree_iter_t iter);  
```


