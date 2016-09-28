#ifndef __RBTREE_STRUCT_H__
#define __RBTREE_STRUCT_H__

enum {
    RBTREE_RED = 0,
    RBTREE_BLACK = 1
};

typedef struct rbtree_node {
    size_t size;
    struct rbtree_node *parent;  /* the lowest 3-bit will be reused, [reserved, index_in_parent, color] */
    struct rbtree_node *child[2];
    size_t id;
    cdata_t key;
    cdata_t val;
} rbtree_node_t;

typedef int (*rbtree_cmp_func)(const cdata_t, const cdata_t);
struct rbtree {
    size_t size;
    struct rbtree_node *root;
    rbtree_cmp_func key_cmp;
};

/* special node used to simplify the implementation */
//rbtree_node_t rbtree_tail = {0, (rbtree_node_t*)1, &rbtree_tail, &rbtree_tail};
//rbtree_node_t rbtree_head = {0, &rbtree_head, &rbtree_tail, &rbtree_tail};
extern rbtree_node_t rbtree_tail;
extern rbtree_node_t rbtree_head;

#endif
