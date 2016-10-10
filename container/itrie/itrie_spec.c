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
    assert(((intptr_t)node & 15) == 0);

    int x = 23;
    void *value = &x;
    node->cell[0].key = (cdata_t)x;
    node->cell[0].val.ptr = value;
    assert(node->cell[0].val.ptr == value);
    assert(!itrie_has_sub(node->cell[0].key));

    struct itrie_node *subnode;
    itrie_node_create(&subnode);
    assert(subnode != 0);
    assert(((intptr_t)node & 15) == 0);

    node->cell[1].key = itrie_encode_sub(25);
    node->cell[1].val.ptr = subnode;
    assert(itrie_has_sub(node->cell[1].key));
    assert(itrie_decode_sub((node->cell[1].key)).i == 25);

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

    itrie_elem_insert(itrie, 0x90103, 4);
    assert(itrie->root->cell[3].val.i == 4);
    assert(itrie_elem_insert(itrie, 0x90103, 8));


    itrie_elem_insert(itrie, 0x80503, 8);
    assert(itrie->root->cell[3].val.i != 4);
    itrie_iter_t iter;
    itrie_iter_create(itrie, &iter);
    assert(itrie_iter_search(itrie, 0x80003, iter));
    assert(!itrie_iter_search(itrie, 0x80503, iter));
    cdata_t key;
    cdata_t val;
    assert(!itrie_iter_item(iter, &key, &val));
    assert(key.i == 0x80503);
    assert(val.i == 8);

    itrie_elem_insert(itrie, 15, 16);
    itrie_elem_insert(itrie, 31, 32);
    itrie_elem_insert(itrie, 63, 64);
    itrie_delete(&itrie);
    puts("verify itrie insert pass!");
    return 1;
}

int main(int argc, char* argv[]) {
    assert(verify_itrie_inline());
    assert(verify_itrie_insert());
    return 0;
}

