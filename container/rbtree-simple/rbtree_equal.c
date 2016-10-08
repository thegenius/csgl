#include <assert.h>
#include "rbtree.h"
#include "rbtree_struct.h"
#include "rbtree_inline.h"
#include "rbtree_equal.h"

static bool rbtree_equal_tree(rbtree_node_t *a, rbtree_node_t *b);

bool rbtree_equal(const rbtree_t *a, const rbtree_t *b) {
    if (a == b) {
        return true;
    }
    if (a == 0 || b == 0) {
        return false;
    }

    if (a->root == b->root) {
        return true;
    }
    if (a->root == 0 || b->root == 0) {
        return false;
    }
    if (a->root == &rbtree_tail || b->root == &rbtree_tail) {
        return false;
    }
    return rbtree_equal_tree(a->root, b->root);
}


bool rbtree_equal_tree(rbtree_node_t *a, rbtree_node_t *b) {
    if (a->id != b->id) {
        return false;
    }
    if (rbtree_color(a) != rbtree_color(b)) {
        return false;
    }
    if (a->child[0] == &rbtree_tail || b->child[0] == &rbtree_tail) {
        if (a->child[0] != b->child[0]) {
            return false;
        }
    }
    if (a->child[1] == &rbtree_tail || b->child[1] == &rbtree_tail) {
        if (a->child[1] != b->child[1]) {
            return false;
        }
        return true;
    }
    bool lequal = rbtree_equal_tree(a->child[0], b->child[0]);
    bool requal = rbtree_equal_tree(a->child[1], b->child[1]);
    if ((!lequal) || (!requal)) {
        return false;
    }
    return true;
}
