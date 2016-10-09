#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <unistd.h>
#include "cdata.h"
#include "itrie.h"
#include "itrie_inline.h"


int verify_itrie_inline() {
    puts("verify itrie inline ....!");
    struct itrie_node *node;
    itrie_node_create(&node);
    assert(((intptr_t)node & 63) == 0);

    int x = 23;
    void *value = &x;
    node->cell[0].val.ptr = value;
    assert(node->cell[0].val.ptr == value);
    assert(!itrie_has_subnode(node->cell[0].val.ptr));

    struct itrie_node *subnode;
    itrie_node_create(&subnode);
    assert(subnode != 0);
    assert(((intptr_t)node & 63) == 0);

    struct itrie_node *encode_subnode = itrie_encode_subnode(subnode);
    node->cell[1].val.ptr = encode_subnode;
    assert(itrie_has_subnode(node->cell[1].val.ptr));
    assert(itrie_decode_subnode((node->cell[1].val.ptr)) == subnode);

    itrie_node_delete(&subnode);
    assert(subnode == 0);
    itrie_node_delete(&node);
    assert(node == 0);
    puts("verify itrie inline pass!");
    return 1;
}

int verify_itrie_insert() {
    puts("verify itrie insert ....!");
    itrie_t itrie;
    itrie_create(&itrie);

    itrie_insert(itrie, 0x90103, 4);
    assert(itrie->root->cell[3].key.i == 0x90103);
    assert(itrie->root->cell[3].val.i == 4);

    itrie_insert(itrie, 0x80003, 8);


    itrie_insert(itrie, 15, 16);
    itrie_insert(itrie, 31, 32);
    itrie_insert(itrie, 63, 64);
    itrie_delete(&itrie);
    puts("verify itrie insert pass!");
    return 1;
}

int main(int argc, char* argv[]) {
    assert(verify_itrie_inline());
    assert(verify_itrie_insert());
    return 0;
}

