#ifndef __ITRIE_INLINE_H__
#define __ITRIE_INLINE_H__

#include <string.h>
#include <stdint.h>
#include <stdalign.h>
#include "itrie_struct.h"

static inline int itrie_node_create(struct itrie_node **node) {
    size_t len = sizeof(struct itrie_node);
    *node = aligned_alloc(len, len);
    memset(*node, 0, len);
    return 0;
}

static inline int itrie_node_delete(struct itrie_node **node) {
    free(*node);
    *node = 0;
    return 0;
}

static inline size_t itrie_get_idx(const cdata_t key) {
    return (size_t)((intptr_t)key.ptr & (ITRIE_NODE_MAX_CELL - 1));
}

static inline bool itrie_has_subnode(struct itrie_node *node) {
    return (bool)((intptr_t)node & 1);
}

static inline struct itrie_node* itrie_encode_subnode(const struct itrie_node *node) {
    return (struct itrie_node*)((intptr_t)node | 1);
}

static inline struct itrie_node* itrie_decode_subnode(const struct itrie_node *node) {
    return (struct itrie_node*)((intptr_t)node & ~1);
}

#endif
