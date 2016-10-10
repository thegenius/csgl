#ifndef __ITRIE_STRUCT_H__
#define __ITRIE_STRUCT_H__

enum {
    ITRIE_MAX_LEVEL = 16,
    ITRIE_NODE_IDX_BITS = 4,
    ITRIE_NODE_MAX_CELL = 16
};

struct itrie_pair {
    cdata_t key;
    cdata_t val;
};

struct itrie_node {
    struct itrie_pair cell[ITRIE_NODE_MAX_CELL];
};

struct itrie_iter_level {
    size_t idx;
    struct itrie_node* ptr;
};

struct itrie_iter {
    int cur;
    cdata_t pre;
    struct itrie_iter_level level[0];
};

struct itrie {
    size_t size;
    size_t max_level;
    struct itrie_node* root;
};

#endif
