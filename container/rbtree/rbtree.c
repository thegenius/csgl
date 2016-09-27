#include "rbtree.h"
#include <assert.h>
#include <stdio.h>

enum {
    RBTREE_RED = 0,
    RBTREE_BLACK = 1 
};

struct rbtree_node {
    size_t size;
    struct rbtree_node *parent;  /* the lowest 3-bit will be reused, [reserved, index_in_parent, color] */
    struct rbtree_node *child[2];
    size_t id;
    cdata_t key;
    cdata_t val;
};
typedef struct rbtree_node rbtree_node_t;

typedef int (*rbtree_key_cmp_func)(const cdata_t, const cdata_t);
struct rbtree {
    size_t size;
    struct rbtree_node *root;
    rbtree_key_cmp_func key_cmp;
};

rbtree_node_t rbtree_tail = {0, (rbtree_node_t*)1, &rbtree_tail, &rbtree_tail};
rbtree_node_t rbtree_head = {0, 0, &rbtree_tail, &rbtree_tail};


/***************************************************************************************
 * internal inline function
 ***************************************************************************************/
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
    node->size = node->child[0]->size + node->child[1]->size + 1;
    child->parent = (rbtree_node_t*)((unsigned long)(node) | rbtree_color(child) | (index << 1));
}

static inline void rbtree_detach_child(rbtree_node_t *child) {
    size_t index = rbtree_get_index(child);
    rbtree_node_t *node = rbtree_parent(child);
    node->child[index] = &rbtree_tail;
    node->size = node->child[0]->size + node->child[1]->size + 1;
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
    (*node)->size = 1;
    (*node)->child[0] = &rbtree_tail;
    (*node)->child[1] = &rbtree_tail;
    return 0;
}

static inline int rbtree_node_delete(rbtree_node_t **node) {
    free(*node);
    *node = 0;
    return 0;
}

static inline void rbtree_adjust_size(rbtree_node_t *node) {
    while (node != &rbtree_head) {
        node->size = node->child[0]->size + node->child[1]->size + 1;
        node = rbtree_parent(node);
    }
}

void rbtree_insert_balance(rbtree_t *tree, rbtree_node_t *node) {
    /* assume the node is the one you just insert into the tree.
       and the color of the node is black. */
    rbtree_node_t *parent, *uncle, *gparent;

    rbtree_set_red(node);
    while (((parent = rbtree_parent(node)) != &rbtree_head) && rbtree_is_red(parent)) {
        /* parent is red, there must be a black gparent and an uncle */
        gparent = rbtree_parent(parent);
        uncle = rbtree_uncle(node);
        assert(rbtree_color(gparent) == RBTREE_BLACK);

        /* parent is red and uncle is red, split the node */
        if (rbtree_is_red(uncle)) {
            rbtree_set_black(uncle);
            rbtree_set_black(parent);
            rbtree_set_red(gparent);
            node = gparent;
            continue;
        }

        /* uncle is black */
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

    /* adjust the size */
    rbtree_adjust_size(node);
    rbtree_set_black(tree->root);
}

static void rbtree_delete_balance(rbtree_t *tree, rbtree_node_t *node) {
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
            node = gparent;
            break;
        }
        if (rbtree_is_red(sibling)) {
            puts("case : borrow from sibling");
            rbtree_rotate_borrow(gparent, parent, node, sibling);
            rbtree_set_black(sibling);
            node = gparent;
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
            node = gparent;
            break;
        }
        if (rbtree_is_red(far_child)) {
            puts("borrow far");
            rbtree_rotate_borrow(gparent, parent, node, sibling);
            rbtree_set_black(far_child);
            node = gparent;
            break;
        }
        puts("case : merge");
        assert(sibling != &rbtree_tail);
        rbtree_set_red(sibling);
        node = parent;
    }
    rbtree_adjust_size(node);
}


int rbtree_elem_delete(rbtree_t *tree, const cdata_t idx, cdata_t *val) {
    rbtree_node_t *node, *sibling, *parent, *uncle, *gparent;
    /* find the node to delete */
    node = tree->root;
    while (node != &rbtree_tail) {
        int cmp = tree->key_cmp(node->key, idx);
        if (cmp == 0) {
            break;
        }
        node = node->child[cmp < 0];
    }
    if (node == &rbtree_tail) {
        return -1;
    }
    *val = node->val;

    /* delete red node, just adjust the size */
    if (rbtree_is_red(node)) {
        puts("red node case, just adjust size");
        rbtree_adjust_size(node);
        return 0;
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
        return 0;
    }



}

int rbtree_check_internal() {
    const int len = 5;

    assert(rbtree_is_black(&rbtree_tail));
    rbtree_node_t *node[len];
    for (int i=0; i<len; ++i) {
        rbtree_node_create(&node[i]);
        node[i]->id = i;
    }

    printf("check set_child ...\n");
    rbtree_attach_child(node[1], 0, node[0]);
    rbtree_attach_child(node[1], 1, node[2]);
    rbtree_attach_child(node[3], 0, node[1]);
    rbtree_attach_child(node[3], 1, node[4]);
    printf("check set_child pass!\n");

    printf("check get parent ...\n");
    assert(rbtree_parent(node[0]) == node[1]);
    assert(rbtree_parent(node[2]) == node[1]);
    assert(rbtree_parent(node[1]) == node[3]);
    assert(rbtree_parent(node[4]) == node[3]);

    assert(rbtree_gparent(node[0]) == node[3]);
    assert(rbtree_gparent(node[2]) == node[3]);

    assert(rbtree_sibling(node[0]) == node[2]);
    assert(rbtree_sibling(node[2]) == node[0]);
    assert(rbtree_sibling(node[1]) == node[4]);
    assert(rbtree_sibling(node[4]) == node[1]);

    assert(rbtree_uncle(node[0]) == node[4]);
    assert(rbtree_uncle(node[2]) == node[4]);


    assert(node[0]->size == 1);
    assert(node[1]->size == 3);
    assert(node[2]->size == 1);
    assert(node[3]->size == 5);
    assert(node[4]->size == 1);

    rbtree_set_black(node[0]);
    rbtree_set_black(node[2]);
    rbtree_set_black(node[3]);

    puts("check rotate right ...");
    rbtree_attach_child(&rbtree_head, 0, node[3]);
    assert(rbtree_parent(node[3]) == &rbtree_head);
    rbtree_rotate_right(0, node[3]);
    assert(rbtree_parent(node[0]) == node[1]);
    assert(rbtree_parent(node[3]) == node[1]);
    assert(rbtree_parent(node[2]) == node[3]);
    assert(rbtree_parent(node[4]) == node[3]);

    assert(rbtree_gparent(node[2]) == node[1]);
    assert(rbtree_gparent(node[4]) == node[1]);

    assert(rbtree_sibling(node[2]) == node[4]);
    assert(rbtree_sibling(node[4]) == node[2]);
    assert(rbtree_sibling(node[0]) == node[3]);
    assert(rbtree_sibling(node[3]) == node[0]);

    assert(rbtree_uncle(node[2]) == node[0]);
    assert(rbtree_uncle(node[4]) == node[0]);
    puts("check rotate right pass!");

    puts("check rotate left ...");
    assert(rbtree_parent(node[1]) == &rbtree_head);
    rbtree_rotate_left(0, node[1]);

    assert(rbtree_parent(node[0]) == node[1]);
    assert(rbtree_parent(node[2]) == node[1]);
    assert(rbtree_parent(node[1]) == node[3]);
    assert(rbtree_parent(node[4]) == node[3]);

    assert(rbtree_gparent(node[0]) == node[3]);
    assert(rbtree_gparent(node[2]) == node[3]);

    assert(rbtree_sibling(node[0]) == node[2]);
    assert(rbtree_sibling(node[2]) == node[0]);
    assert(rbtree_sibling(node[1]) == node[4]);
    assert(rbtree_sibling(node[4]) == node[1]);

    assert(rbtree_uncle(node[0]) == node[4]);
    assert(rbtree_uncle(node[2]) == node[4]);

    printf("check borrow ...\n");
    rbtree_rotate_borrow(&rbtree_head, node[3], node[4], node[1]);
    assert(rbtree_parent(node[1]) == &rbtree_head);
    assert(rbtree_parent(node[0]) == node[1]);
    assert(rbtree_parent(node[3]) == node[1]);
    assert(rbtree_parent(node[2]) == node[3]);
    assert(rbtree_parent(node[4]) == node[3]);

    assert(rbtree_gparent(node[2]) == node[1]);
    assert(rbtree_gparent(node[4]) == node[1]);

    assert(rbtree_sibling(node[2]) == node[4]);
    assert(rbtree_sibling(node[4]) == node[2]);
    assert(rbtree_sibling(node[0]) == node[3]);
    assert(rbtree_sibling(node[3]) == node[0]);

    assert(rbtree_uncle(node[2]) == node[0]);
    assert(rbtree_uncle(node[4]) == node[0]);

    rbtree_rotate_borrow(&rbtree_head, node[1], node[0], node[3]);
    assert(rbtree_parent(node[3]) == &rbtree_head);
    assert(rbtree_parent(node[0]) == node[1]);
    assert(rbtree_parent(node[2]) == node[1]);
    assert(rbtree_parent(node[1]) == node[3]);
    assert(rbtree_parent(node[4]) == node[3]);

    assert(rbtree_gparent(node[0]) == node[3]);
    assert(rbtree_gparent(node[2]) == node[3]);

    assert(rbtree_sibling(node[0]) == node[2]);
    assert(rbtree_sibling(node[2]) == node[0]);
    assert(rbtree_sibling(node[1]) == node[4]);
    assert(rbtree_sibling(node[4]) == node[1]);

    assert(rbtree_uncle(node[0]) == node[4]);
    assert(rbtree_uncle(node[2]) == node[4]);
    printf("check borrow pass!\n");

    printf("check return ...\n");
    rbtree_rotate_return(&rbtree_head, node[3], node[1], node[4]); 
    assert(rbtree_parent(node[1]) == &rbtree_head);
    assert(rbtree_parent(node[0]) == node[1]);
    assert(rbtree_parent(node[3]) == node[1]);
    assert(rbtree_parent(node[2]) == node[3]);
    assert(rbtree_parent(node[4]) == node[3]);

     assert(rbtree_gparent(node[2]) == node[1]);
    assert(rbtree_gparent(node[4]) == node[1]);

    assert(rbtree_sibling(node[2]) == node[4]);
    assert(rbtree_sibling(node[4]) == node[2]);
    assert(rbtree_sibling(node[0]) == node[3]);
    assert(rbtree_sibling(node[3]) == node[0]);

    assert(rbtree_uncle(node[2]) == node[0]);
    assert(rbtree_uncle(node[4]) == node[0]);

    printf("check return pass!\n");

    printf("check insert balance ...\n");
    rbtree_set_black(node[0]);
    rbtree_set_black(node[1]);
    rbtree_set_black(node[3]);
    rbtree_set_red(node[2]);
    rbtree_set_red(node[4]);

    rbtree_t tree;
    tree.root = node[1];
    const int insert_len = 5;
    rbtree_node_t *insert_node[insert_len];
    for (int i=0; i<insert_len; ++i) {
        rbtree_node_create(&insert_node[i]);
        rbtree_set_black(insert_node[i]);
        insert_node[i]->id = len + i;
    }

    rbtree_attach_child(node[2], 0, insert_node[0]);
    rbtree_insert_balance(&tree, insert_node[0]);

    assert(rbtree_parent(insert_node[0]) == node[2]);
    assert(rbtree_is_red(insert_node[0]));
    assert(rbtree_is_black(node[2]));
    assert(rbtree_is_black(node[4]));
    assert(rbtree_is_red(node[3]));

    rbtree_attach_child(insert_node[0], 1, insert_node[1]);
    rbtree_insert_balance(&tree, insert_node[1]);
    assert(rbtree_parent(insert_node[1]) == node[3]);
    assert(rbtree_parent(insert_node[0]) == insert_node[1]);
    assert(rbtree_parent(node[2]) == insert_node[1]);
    assert(rbtree_is_red(node[2]));
    assert(rbtree_is_red(insert_node[0]));
    assert(rbtree_is_black(insert_node[1]));

    rbtree_attach_child(node[4], 1, insert_node[2]);
    rbtree_insert_balance(&tree, insert_node[2]);
    rbtree_attach_child(insert_node[2], 1, insert_node[3]);
    rbtree_insert_balance(&tree, insert_node[3]);
    assert(rbtree_parent(insert_node[3]) == insert_node[2]);
    assert(rbtree_parent(node[4]) == insert_node[2]);
    assert(rbtree_is_red(node[4]));
    assert(rbtree_is_red(insert_node[3]));
    assert(rbtree_is_black(insert_node[2]));
    puts("check insert balance pass!");


    assert(rbtree_is_red(node[3]));
    puts("check delete balance ...");
    rbtree_delete_balance(&tree, insert_node[3]);
    rbtree_detach_child(insert_node[3]);

    rbtree_delete_balance(&tree, node[4]);
    rbtree_detach_child(node[4]);

    assert(rbtree_is_red(node[3]));
    rbtree_delete_balance(&tree, insert_node[2]);
    assert(rbtree_is_red(node[3]));
    rbtree_detach_child(insert_node[2]);
    assert(rbtree_is_red(node[3]));
    assert(rbtree_parent(node[3]) == insert_node[1]);
    assert(rbtree_is_black(node[1]));


    assert(rbtree_parent(node[0]) == node[1]);
    rbtree_delete_balance(&tree, node[0]);
    rbtree_detach_child(node[0]);
    assert(rbtree_parent(node[1]) == insert_node[0]);
    assert(rbtree_parent(node[3]) == insert_node[1]);
    assert(rbtree_parent(insert_node[1]) == insert_node[0]);
    assert(rbtree_is_red(node[3]));
    assert(rbtree_get_index(node[3]) == 1);

    rbtree_delete_balance(&tree, node[1]);
    rbtree_detach_child(node[1]);
    assert(rbtree_is_black(node[3]));
    assert(rbtree_parent(node[3]) == insert_node[1]);

    rbtree_delete_balance(&tree, node[3]);
    rbtree_detach_child(node[3]);
    assert(rbtree_is_red(insert_node[0]));

    rbtree_delete_balance(&tree, insert_node[1]);
    rbtree_detach_child(insert_node[1]);
    assert(tree.root == insert_node[0]);

    assert(rbtree_tail.size == 0);
    rbtree_delete_balance(&tree, insert_node[0]);
    rbtree_detach_child(insert_node[0]);
    assert(tree.root == &rbtree_tail);

    puts("check delete balance pass!");

    for (int i=0; i<len; ++i) {
        rbtree_node_delete(&node[i]);
    }
    printf("rbtree_tail.size:%d\n", rbtree_tail.size);
    assert(rbtree_tail.size == 0);
    assert(rbtree_is_black(&rbtree_tail));
    return 1;
}




