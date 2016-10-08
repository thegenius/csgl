#ifndef __RBTREE_INLINE_H__
#define __RBTREE_INLINE_H__
#include "rbtree_struct.h"

static inline rbtree_node_t* rbtree_parent(rbtree_node_t *node) {
    return (rbtree_node_t*)((unsigned long)(node->parent) & ~3);
}

static inline int rbtree_has_parent(rbtree_node_t *node) {
    return (bool)(node->parent);
}

static inline rbtree_node_t* rbtree_gparent(rbtree_node_t *node) {
    rbtree_node_t *parent = (rbtree_node_t*)((unsigned long)(node->parent) & ~3);
    rbtree_node_t *gparent = (rbtree_node_t*)((unsigned long)(parent->parent) & ~3);
    return gparent;
}

static inline int rbtree_has_gparent(rbtree_node_t *node) {
    rbtree_node_t *parent = (rbtree_node_t*)((unsigned long)(node->parent) & ~3);
    return (bool)(parent->parent);
}

static inline rbtree_node_t* rbtree_sibling(rbtree_node_t *node) {
    int sibling_index = (int)(!(((unsigned long)(node->parent) & 2) >> 1));
    rbtree_node_t *parent = (rbtree_node_t*)((unsigned long)(node->parent) & ~3);
    return parent->child[sibling_index];
}

static inline int rbtree_has_sibling(rbtree_node_t *node) {
    return (bool)rbtree_sibling(node);
}

static inline rbtree_node_t* rbtree_uncle(rbtree_node_t *node) {
    rbtree_node_t *parent = (rbtree_node_t*)((unsigned long)(node->parent) & ~3);
    rbtree_node_t *gparent = (rbtree_node_t*)((unsigned long)(parent->parent) & ~3);
    int uncle_index = (int)(!(((unsigned long)(parent->parent) & 2) >> 1));
    return gparent->child[uncle_index];
}

static inline int rbtree_has_uncle(rbtree_node_t *node) {
    return (bool)rbtree_uncle(node);
}

static inline int rbtree_color(rbtree_node_t *node) {
    return (unsigned long)(node->parent) & 1;
}

static inline int rbtree_is_red(rbtree_node_t *node) {
    return !((unsigned long)(node->parent) & 1);
}

static inline int rbtree_is_black(rbtree_node_t *node) {
    return ((unsigned long)(node->parent) & 1);
}

static inline void rbtree_set_red(rbtree_node_t *node) {
    node->parent = (rbtree_node_t*)((unsigned long)(node->parent) & ~1);
}

static inline void rbtree_set_black(rbtree_node_t *node) {
    node->parent = (rbtree_node_t*)((unsigned long)(node->parent) | 1);
}

static inline void rbtree_set_color(rbtree_node_t *node, const int color) {
    node->parent = (rbtree_node_t*)(((unsigned long)(node->parent) & ~1) | color);
}

static inline void rbtree_set_parent(rbtree_node_t *node, rbtree_node_t *parent) {
    node->parent = (rbtree_node_t*)(((unsigned long)(node->parent) & 3) | (unsigned long)parent);
}

static inline size_t rbtree_get_index(rbtree_node_t *node) {
    return (size_t)(((unsigned long)(node->parent) & 2) >> 1);
}

static inline void rbtree_attach_child(rbtree_node_t *node, const size_t index, rbtree_node_t *child) {
    node->child[index] = child;
    child->parent = (rbtree_node_t*)((unsigned long)(node) | rbtree_color(child) | (index << 1));
}

static inline void rbtree_detach_child(rbtree_node_t *child) {
    size_t index = rbtree_get_index(child);
    rbtree_node_t *node = rbtree_parent(child);
    node->child[index] = &rbtree_tail;
}

static inline void rbtree_rotate_left(rbtree_t *tree, rbtree_node_t *node) {
    rbtree_node_t *right = node->child[1];
    rbtree_node_t *parent = rbtree_parent(node);
    size_t index = rbtree_get_index(node);
    rbtree_attach_child(node, 1, right->child[0]);
    rbtree_attach_child(right, 0, node);
    rbtree_attach_child(parent, index, right);
}

static inline void rbtree_rotate_right(rbtree_t *tree, rbtree_node_t *node) {
    rbtree_node_t *left = node->child[0];
    rbtree_node_t *parent = rbtree_parent(node);
    size_t index = rbtree_get_index(node);
    rbtree_attach_child(node, 0, left->child[1]);
    rbtree_attach_child(left, 1, node);
    rbtree_attach_child(parent, index, left);
}

static inline void rbtree_rotate_borrow(rbtree_node_t *gparent, rbtree_node_t *parent, rbtree_node_t *node, rbtree_node_t *sibling) {
    size_t node_index = rbtree_get_index(node);
    size_t sibling_index = (int)(!node_index);
    size_t parent_index = rbtree_get_index(parent);
    rbtree_attach_child(parent, sibling_index, sibling->child[node_index]);
    assert(sibling != &rbtree_tail);
    rbtree_attach_child(sibling, node_index, parent);
    rbtree_attach_child(gparent, parent_index, sibling);
}

static inline void rbtree_rotate_return(rbtree_node_t *gparent, rbtree_node_t *parent, rbtree_node_t *node, rbtree_node_t *sibling) {
    size_t node_index = rbtree_get_index(node);
    size_t sibling_index = (int)(!node_index);
    size_t parent_index = rbtree_get_index(parent);
    rbtree_attach_child(parent, node_index, node->child[sibling_index]);
    rbtree_attach_child(node, sibling_index, parent);
    rbtree_attach_child(gparent, parent_index, node);
}

static inline int rbtree_node_create(rbtree_node_t **node){
    *node = (rbtree_node_t*)calloc(1, sizeof(rbtree_node_t));
    (*node)->child[0] = &rbtree_tail;
    (*node)->child[1] = &rbtree_tail;
    (*node)->parent = &rbtree_head;
    rbtree_set_black(*node);
    return 0;
}

static inline int rbtree_node_delete(rbtree_node_t **node) {
    free(*node);
    *node = 0;
    return 0;
}

#endif
