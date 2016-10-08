#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "rbtree.h"
#include "rbtree_build.h"
#include "rbtree_struct.h"
#include "rbtree_inline.h"

extern int rbtree_node_create(rbtree_node_t **node);
extern int rbtree_node_delete(rbtree_node_t **node);

static void skip_white_space();
static int rbtree_build_tree(char** tree_def, rbtree_node_t **node);
static int rbtree_build_nil(char **tree_def, rbtree_node_t** node);
static int rbtree_build_node(char** tree_def, rbtree_node_t **node);
static int rbtree_build_subtree(char **tree_def, rbtree_node_t **lchild, rbtree_node_t **rchild);
static int rbtree_build_id(char **tree_def, int* id);


int rbtree_build(char *tree_def, rbtree_t **tree) {
    rbtree_create(tree, 0);
    char *str_def = tree_def;
    return rbtree_build_tree(&str_def, &((*tree)->root));
}

void skip_white_space(char **str) {
    while (**str == ' ') {
        ++ *str;
    }
}

int rbtree_build_id(char **tree_def, int* id) {
    skip_white_space(tree_def);
    int num = 0;
    while (**tree_def >= '0' && **tree_def <= '9') {
        num = num * 10 + **tree_def - '0';
        ++ *tree_def;
    }
    *id = num;
    return 0;
}

int rbtree_build_nil(char **tree_def, rbtree_node_t** node) {
    skip_white_space(tree_def);
    if (**tree_def != 'n' || *(*(tree_def)+1) != 'i' || *(*(tree_def)+2) != 'l') {
        printf("expect nil, but get %c%c%c\n", tree_def[0], tree_def[1], tree_def[2]);
        return -1;
    }
    *tree_def += 3;
    *node = &rbtree_tail;
    return 0;
}

int rbtree_build_node(char** tree_def, rbtree_node_t **node) {
    /* node ::= { (color, id), subtree } */
    int err = 0;
    skip_white_space(tree_def);
    if (**tree_def != '{') {
        printf("expect ) when parse node, but get %c\n node ::= {(color,id), subtree}\n", **tree_def);
        return -1;
    }
    ++ *tree_def; // eat {

    skip_white_space(tree_def);
    if (**tree_def != '(') {
        printf("expect ( when parse node, but get %c\n node ::= {(color,id), subtree}\n", **tree_def);
        return -1;
    }
    ++ *tree_def; // eat (

    skip_white_space(tree_def);
    int color = **tree_def - '0';
    ++ *tree_def; // eat color

    skip_white_space(tree_def);
    if (**tree_def != ',') {
        printf("expect , when parse node, but get %c\n node ::= {(color,id), subtree}\n", **tree_def);
        return -1;
    }
    ++ *tree_def; // eat ,

    int id = 0;
    err = rbtree_build_id(tree_def, &id);
    if (err) {
        return -1;
    }

    // create node
    rbtree_node_create(node);
    rbtree_set_color(*node, color);
    (*node)->id = id;

    skip_white_space(tree_def);
    if (**tree_def != ')') {
        rbtree_node_delete(node);
        printf("expect ) when parse node, but get %c\n node ::= {(color,id), subtree}\n", **tree_def);
        return -1;
    }
    ++ *tree_def; // eat )

    skip_white_space(tree_def);
    if (**tree_def != ',') {
        rbtree_node_delete(node);
        printf("expect , when parse node, but get %c\n node ::= {(color,id), subtree}\n", **tree_def);
        return -1;
    }
    ++ *tree_def; // eat ,

    err = rbtree_build_subtree(tree_def, &((*node)->child[0]), &((*node)->child[1]));
    if (err) {
        rbtree_node_delete(node);
        return -1;
    }
    rbtree_attach_child(*node, 0, (*node)->child[0]);
    rbtree_attach_child(*node, 1, (*node)->child[1]);


    skip_white_space(tree_def);
    if (**tree_def != '}') {
        rbtree_node_delete(node);
        printf("expect } when parse node, but get %c\n node ::= {(color,id), subtree}\n", **tree_def);
        return -1;
    }
    ++ *tree_def; // eat }

    return 0;
}

int rbtree_build_subtree(char **tree_def, rbtree_node_t **lchild, rbtree_node_t **rchild) {
    int err = 0;
    skip_white_space(tree_def);
    if (**tree_def != '[') {
        printf("expect [ when parse subtree but get %c\n subtree ::= [tree, tree]\n", **tree_def);
        return -1;
    }
    ++ *tree_def; // eat [

    err = rbtree_build_tree(tree_def, lchild);
    if (err) {
        return -1;
    }

    skip_white_space(tree_def);
    if (**tree_def != ',') {
        printf("expect , when parse subtree but get %c\n subtree ::= [tree, tree]\n", **tree_def);
        return -1;
    }
    ++ *tree_def; //eat ,

    err = rbtree_build_tree(tree_def, rchild);
    if (err) {
        return -1;
    }

    skip_white_space(tree_def);
    if (**tree_def != ']') {
        printf("expect ] when parse subtree but get %c\n subtree ::= [tree, tree]\n", **tree_def);
        return -1;
    }
    ++ *tree_def; // eat ]
    return 0;
}

int rbtree_build_tree(char** tree_def, rbtree_node_t **node) {
    skip_white_space(tree_def);
    if (**tree_def == 'n') {
        return rbtree_build_nil(tree_def, node);
    }
    if (**tree_def == '{') {
        return rbtree_build_node(tree_def, node);
    }

    printf("expect nil or {  when parse tree but get %c\n tree ::= nil | node\n", **tree_def);
    return -1;
}


