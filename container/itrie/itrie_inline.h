#ifndef __ITRIE_INLINE_H__
#define __ITRIE_INLINE_H__

#include <string.h>
#include <stdint.h>
#include <stdalign.h>
#include "itrie_struct.h"

#define itrie_has_sub(key) __itrie_has_sub((cdata_t)key)
#define itrie_encode_sub(key) __itrie_encode_sub((cdata_t)key)
#define itrie_decode_sub(key) __itrie_decode_sub((cdata_t)key)

#define itrie_has_val(key) __itrie_has_val((cdata_t)key)
#define itrie_encode_val(key) __itrie_encode_val((cdata_t)key)
#define itrie_decode_val(key) __itrie_decode_val((cdata_t)key)

enum {
    HAS_VAL_FLAG = (uintptr_t)1 << (sizeof(uintptr_t) * 8 - 1),
    HAS_SUB_FLAG = (uintptr_t)1 << (sizeof(uintptr_t) * 8 - 2),
    HAS_SUB_NODE = (uintptr_t)3 << (sizeof(uintptr_t) * 8 - 2)
};

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

static inline int itrie_iter_create(struct itrie *itrie, struct itrie_iter **iter) {
    *iter = (struct itrie_iter*)calloc(1, sizeof(uintptr_t)*(itrie->max_level));
    return 0;
}

static inline int itrie_iter_delete(struct itrie_iter **iter) {
    free(*iter);
    *iter = 0;
    return 0;
}

static inline int itrie_iter_clear(struct itrie_iter *iter) {
    iter->pre = (cdata_t)0;
    iter->cur = -1;
    return 0;
}

static inline int itrie_iter_push(struct itrie_iter *iter, struct itrie_node *node, size_t idx) {
    ++ iter->cur;
    iter->pre = (cdata_t)(iter->pre.uintptr | (idx << ((iter->cur) << 2)));
    iter->level[iter->cur].idx = idx;
    iter->level[iter->cur].ptr = node;
    return 0;
}

static inline int itrie_iter_pull(struct itrie_iter *iter, struct itrie_node **node, size_t *idx) {
    struct itrie_iter_level level = iter->level[iter->cur];
    *node = level.ptr;
    *idx = level.idx;
    -- iter->cur;
    return 0;
}

static inline int itrie_iter_item(struct itrie_iter *iter, cdata_t *key, cdata_t *val) {
    if (iter->cur < 0) {
        return -1;
    }
    struct itrie_iter_level level = iter->level[iter->cur];
    struct itrie_node *node = level.ptr;
    size_t idx = level.idx;
    *key = iter->pre;
    *val = node->cell[idx].val;
    return 0;
}

static inline size_t itrie_get_idx(const cdata_t key) {
    return (size_t)((uintptr_t)key.ptr & (ITRIE_NODE_MAX_CELL - 1));
}

static inline bool __itrie_has_val(const cdata_t key) {
    return (bool)((uintptr_t)key.ptr & HAS_VAL_FLAG);
}

static inline cdata_t __itrie_encode_val(const cdata_t key) {
    return (cdata_t)((uintptr_t)key.ptr | HAS_VAL_FLAG);
}

static inline cdata_t __itrie_decode_val(const cdata_t key) {
    return (cdata_t)((uintptr_t)key.ptr & ~HAS_VAL_FLAG);
}

static inline bool __itrie_has_sub(const cdata_t key) {
    return (bool)((uintptr_t)key.ptr & HAS_SUB_FLAG);
}

static inline cdata_t __itrie_encode_sub(const cdata_t key) {
    return (cdata_t)((uintptr_t)key.ptr | HAS_SUB_FLAG);
}

static inline cdata_t __itrie_decode_sub(const cdata_t key) {
    return (cdata_t)((uintptr_t)key.ptr & ~HAS_SUB_FLAG);
}

#endif
