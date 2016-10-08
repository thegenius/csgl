#ifndef __RBTREE_BUILD_H__
#define __RBTREE_BUILD_H__
#include "rbtree_struct.h"

/********************************************************
        tree ::= nil | node
        node ::= { (color, id), subtree }
        subtree ::= [ tree, tree ]
        color ::= 0 | 1
        id ::= digit
*********************************************************/

extern int rbtree_build(char *tree_def, rbtree_t **tree);

#endif
