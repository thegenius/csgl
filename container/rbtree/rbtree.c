#include <assert.h>
#include <stdio.h>
#include "rbtree.h"
#include "rbtree_struct.h"
#include "rbtree_inline.h"

/* special node used to simplify the implementation */
rbtree_node_t rbtree_tail = {0, (rbtree_node_t*)1, &rbtree_tail, &rbtree_tail};
rbtree_node_t rbtree_head = {0, &rbtree_head, &rbtree_tail, &rbtree_tail};

static int default_cmp (const cdata_t a, const cdata_t b) {
    return a.i - b.i;
}

int rbtree_create(rbtree_t **tree) {
    *tree = (rbtree_t*)calloc(1, sizeof(rbtree_t));
    (*tree)->root = &rbtree_tail;
    (*tree)->key_cmp = default_cmp;
    return 0;
}

int rbtree_delete(rbtree_t **tree) {
    free(*tree);
    *tree = 0;
    return 0;
}

static inline void rbtree_adjust_size(rbtree_node_t *node) {
    puts("adjust size");
    while (node != &rbtree_head) {
        node->size = node->child[0]->size + node->child[1]->size + 1;
        node = rbtree_parent(node);
    }
    puts("adjust size finish");
}

static void rbtree_maintain(rbtree_t *tree, rbtree_node_t *node) {
    /* adjust the size */
    assert(node != &rbtree_head);
    rbtree_node_t *parent = rbtree_parent(node);
    while (parent != &rbtree_head) {
        parent->size = parent->child[0]->size + parent->child[1]->size + 1;
        node = parent;
        parent = rbtree_parent(parent);
    }
    /* adjust the root */
    tree->root = node;
    rbtree_set_black(tree->root);
}

void rbtree_insert_balance(rbtree_t *tree, rbtree_node_t *node) {
    /* assume the node is the one you just insert into the tree. */
    rbtree_node_t *parent, *uncle, *gparent;
    rbtree_set_red(node);
    while (((parent = rbtree_parent(node)) != &rbtree_head) && rbtree_is_red(parent)) {

        /* parent is red, there must be a black gparent */
        gparent = rbtree_parent(parent);
        uncle = rbtree_uncle(node);
        assert(rbtree_is_black(gparent));

        /* parent is red and uncle is red, split the node */
        if (rbtree_is_red(uncle)) {
            rbtree_set_black(uncle);
            rbtree_set_black(parent);
            rbtree_set_red(gparent);
            node = gparent;
            continue;
        }

        /* gparent is black, parent is red, uncle is black */
        rbtree_set_red(gparent);
        size_t node_index = rbtree_get_index(node);
        size_t parent_index = rbtree_get_index(parent);
        rbtree_node_t *sibling = rbtree_sibling(node);
        rbtree_node_t *ggparent = rbtree_parent(gparent);
        if (node_index != parent_index) {
            rbtree_rotate_return(gparent, parent, node, sibling);
            rbtree_rotate_return(ggparent, gparent, node, uncle);
            rbtree_set_black(node);
            rbtree_set_red(gparent);
        } else {
            rbtree_rotate_return(ggparent, gparent, parent, uncle);
            rbtree_set_black(parent);
            rbtree_set_red(gparent);
        }
        node = gparent;
        break;
    }
    rbtree_maintain(tree, node);
}

void rbtree_delete_balance(rbtree_t *tree, rbtree_node_t *node) {
    /* the node is the one that you want to borrow a black node from parent */
    /* do this operation before you really want to delete a node */
    rbtree_node_t *sibling, *parent, *uncle, *gparent;
    printf("delete [%d]\n", node->id);
    /* delete red node, just adjust the size */
    if (rbtree_is_red(node)) {
        puts("red node case, just adjust size");
        rbtree_adjust_size(node);
        return;
    }

    /* delete the root node */
    parent = rbtree_parent(node);
    if (parent == &rbtree_head) {
        puts("root case");
        rbtree_node_t *lchild = node->child[0];
        rbtree_node_t *rchild = node->child[1];
        if (lchild != &rbtree_tail) {
            tree->root = lchild;
        } else {
            tree->root = rchild;
        }
        rbtree_attach_child(&rbtree_head, 0, tree->root);
        rbtree_set_black(tree->root);
        return;
    }

    while ((parent = rbtree_parent(node)) != &rbtree_head) {
        /* if the node is black, and it has parent, there must be a sibling */
        rbtree_node_t *gparent = rbtree_parent(parent);
        rbtree_node_t *sibling = rbtree_sibling(node);
        if (rbtree_is_red(parent)) {
            puts("case : borrow from parent");
            rbtree_rotate_borrow(gparent, parent, node, sibling);
            node = parent;
            break;
        }
        if (rbtree_is_red(sibling)) {
            puts("case : borrow from sibling");
            rbtree_rotate_borrow(gparent, parent, node, sibling);
            rbtree_set_black(sibling);
            node = parent;
            break;
        }
        size_t index = rbtree_get_index(node);
        rbtree_node_t *near_child = sibling->child[index];
        rbtree_node_t *far_child = sibling->child[!index];
        if (rbtree_is_red(near_child)) {
            puts("borrow near");
            rbtree_rotate_return(parent, sibling, near_child, far_child);
            rbtree_rotate_borrow(gparent, parent, node, near_child);
            rbtree_set_black(near_child);
            node = parent;
            break;
        }
        if (rbtree_is_red(far_child)) {
            puts("borrow far");
            rbtree_rotate_borrow(gparent, parent, node, sibling);
            rbtree_set_black(far_child);
            node = parent;
            break;
        }
        puts("case : merge");
        assert(sibling != &rbtree_tail);
        rbtree_set_red(sibling);
        node = parent;
    }

    /* adjust the size */
    parent = rbtree_parent(node);
    assert(parent);
    while (parent != &rbtree_head) {
        parent->size = parent->child[0]->size + parent->child[1]->size + 1;
        node = parent;
        parent = rbtree_parent(parent);
    }
    tree->root = node;
    rbtree_set_black(tree->root);
}

int rbtree_iter_head(const rbtree_t *tree, rbtree_iter_t *iter) {
    rbtree_node_t *node = tree->root;
    while (node->child[0] != &rbtree_tail) {
        node = node->child[0];
    }
    *iter = node;
    return *iter == &rbtree_tail;
}

int rbtree_iter_tail(const rbtree_t *tree, rbtree_iter_t *iter) {
    rbtree_node_t *node = tree->root;
    while (node->child[1] != &rbtree_tail) {
        node = node->child[1];
    }
    *iter = node;
    return *iter == &rbtree_tail;
}

int rbtree_iter_prev(const rbtree_t *tree, rbtree_iter_t *iter) {
    assert(*iter != &rbtree_head);
    assert(*iter != &rbtree_tail);
    rbtree_node_t *node = *iter;
    if (node->child[0] != &rbtree_tail) {
        node = node->child[0];
        while (node->child[1] != &rbtree_tail) {
            node = node->child[1];
        }
        *iter = node;
        return 0;
    }
    rbtree_node_t *parent = rbtree_parent(node);
    while (parent != &rbtree_head) {
        if (rbtree_get_index(node) == 1) {
            *iter = parent;
            return 0;
        }
        node = parent;
        parent = rbtree_parent(node);
    }
    return -1;
}

int rbtree_iter_next(const rbtree_t *tree, rbtree_iter_t *iter) {
    assert(*iter != &rbtree_head);
    assert(*iter != &rbtree_tail);
    rbtree_node_t *node = *iter;
    if (node->child[1] != &rbtree_tail) {
        node = node->child[1];
        while (node->child[0] != &rbtree_tail) {
            node = node->child[0];
        }
        *iter = node;
        return 0;
    }
    rbtree_node_t *parent = rbtree_parent(node);
    while (parent != &rbtree_head) {
        if (rbtree_get_index(node) == 0) {
            *iter = parent;
            return 0;
        }
        node = parent;
        parent = rbtree_parent(node);
    }
    return -1;
}

int __rbtree_iter_index(const rbtree_t *tree, const cdata_t idx, rbtree_iter_t *iter) {
    rbtree_node_t *node = tree->root;
    if (node == &rbtree_tail) {
        return -1;
    }
    size_t size = idx.i;
    while (node != &rbtree_tail) {
        size_t index = node->child[0]->size;
        if (size < index) {
            node = node->child[0];
            continue;
        }
        if (size > index) {
            node = node->child[1];
            size = size - index - 1;
            continue;
        }
        *iter = node;
        return 0;
    }
    return -1;
}

int rbtree_iter_value(const rbtree_t *tree, const cdata_t key, rbtree_iter_t *iter) {
    rbtree_node_t *node = tree->root;
    if (node == &rbtree_tail) {
        return -1;
    }
    size_t size = key.i;
    while (node != &rbtree_tail) {
        int cmp = tree->key_cmp(key, node->key);
        if (cmp < 0) {
            node = node->child[0];
            continue;
        }
        if (cmp > 0) {
            node = node->child[1];
            continue;
        }
        *iter = node;
        return 0;
    }
    return -1;
}


int __rbtree_elem_insert(rbtree_t *tree, const cdata_t key, const cdata_t val) {
    rbtree_node_t *new_node;
    rbtree_node_create(&new_node);
    new_node->key = key;
    new_node->val = val;

    /* empty tree */
    rbtree_node_t *node = tree->root;
    if (node == &rbtree_tail) {
        tree->root = new_node;
        return 0;
    }
    /* find the right place to insert */
    size_t size = key.i;
    rbtree_node_t *parent;
    int cmp;
    do {
        cmp = tree->key_cmp(key, node->key);
        parent = node;
        if (cmp <= 0) {
            node = node->child[0];
        } else {
            node = node->child[1];
        }
    } while (node != &rbtree_tail);
    /* insert the node */
    rbtree_attach_child(parent, cmp>0, new_node);
    rbtree_insert_balance(tree, new_node);
    return 0;
}

int rbtree_elem_update(rbtree_t *tree, const cdata_t key, const cdata_t val) {
    rbtree_iter_t iter;
    int ret = rbtree_iter_value(tree, key, &iter);
    if (ret == 0) {
        iter->val = val;
    }
    return ret;
}

int __rbtree_elem_delete(rbtree_t *tree, const cdata_t idx, cdata_t *val) {
    rbtree_node_t *node, *sibling, *parent, *uncle, *gparent;
    printf("elem delete key :%d\n", idx.i);
    /* find the node to delete */
    node = tree->root;
    while (node != &rbtree_tail) {
        int cmp = tree->key_cmp(idx, node->key);
        if (cmp == 0) {
            break;
        }
        node = node->child[cmp > 0];
    }
    if (node == &rbtree_tail) {
        return -1;
    }

    /* record the value */
    *val = node->val;

    do {
        parent = rbtree_parent(node);
        rbtree_node_t *lchild = node->child[0];
        rbtree_node_t *rchild = node->child[1];
        if (lchild == &rbtree_tail) {
            int node_color = rbtree_color(node);
            int child_color = rbtree_color(rchild);
            rbtree_attach_child(parent, rbtree_get_index(node), rchild);
            rbtree_set_color(rchild, node_color);
            rbtree_delete_balance(tree, rchild);
            rbtree_set_color(rchild, child_color);
            return 0;
        }

        if (rchild == &rbtree_tail) {
            int node_color = rbtree_color(node);
            int child_color = rbtree_color(rchild);
            rbtree_attach_child(parent, rbtree_get_index(node), lchild);
            rbtree_set_color(rchild, node_color);
            rbtree_delete_balance(tree, lchild);
            rbtree_set_color(rchild, child_color);
            return 0;
        }

        /* the node has both lchild and rchild, find a node to replace */
        rbtree_node_t *replace;
        if (lchild->size > rchild->size) {
            replace = lchild;
            while (replace->child[1] != &rbtree_tail) {
                replace = replace->child[1];
            }
        } else {
            replace = rchild;
            while (replace->child[0] != &rbtree_tail) {
                replace = replace->child[0];
            }
        }
        node->key = replace->key;
        node->val = replace->val;
        node = replace;
    } while (1);
}

int rbtree_elem_remove(rbtree_t *tree, const cdata_t key, cdata_t *val) {
    return 0;
}



