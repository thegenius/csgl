#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "itrie.h"
#include "itrie_struct.h"
#include "itrie_inline.h"


int itrie_create(itrie_t *itrie) {
    *itrie = (itrie_t)calloc(1, sizeof(itrie_t));
    itrie_node_create(&((*itrie)->root));
    (*itrie)->max_level = sizeof(void*) * 2;
    return 0;
}

void itrie_delete_iter(itrie_node_t node) {
    for (int i=0; i<ITRIE_NODE_MAX_CELL; ++i) {
        if (itrie_has_subnode(node->cell[i].val.ptr)) {
            itrie_node_t subnode = itrie_decode_subnode(node->cell[i].val.ptr);
            itrie_delete_iter(subnode);
        }
    }
    itrie_node_delete(&node);
}

int itrie_delete(itrie_t *itrie) {
    itrie_delete_iter((*itrie)->root);
    free(*itrie);
    *itrie = 0;
    return 0;
}

size_t itrie_size(itrie_t itrie) {
    return itrie->size;
}

int __itrie_insert(itrie_t itrie, cdata_t key, const cdata_t val) {
    size_t idx, old_idx, new_idx;
    itrie_node_t node = itrie->root;
    for (int i=0; i<itrie->max_level; ++i) {
        idx = itrie_get_idx(key);
        itrie_pair_t cell = node->cell[idx];
        if (itrie_has_subnode(cell.val.ptr)) {
            node = itrie_decode_subnode(cell.val.ptr);
            key = (cdata_t)((uintptr_t)key.ptr >> ITRIE_NODE_IDX_BITS);
        } else {
            if (cell.val.ptr == 0) {
                node->cell[idx].key = key;
                node->cell[idx].val = val;
                ++ itrie->size;
                return 0;
            }
            if (cell.key.ptr == key.ptr) {
                return -1;
            }

            cdata_t old_key = (cdata_t)((uintptr_t)cell.key.ptr >> (i*ITRIE_NODE_IDX_BITS));
            cdata_t new_key = key;
            itrie_node_t subnode;
            do {
                itrie_node_create(&subnode);
                itrie_node_t encoded_subnode = itrie_encode_subnode(subnode);
                node->cell[idx].val.ptr = encoded_subnode;
                old_key = (cdata_t)((uintptr_t)old_key.ptr >> ITRIE_NODE_IDX_BITS);
                new_key = (cdata_t)((uintptr_t)new_key.ptr >> ITRIE_NODE_IDX_BITS);
                old_idx = itrie_get_idx(old_key);
                new_idx = itrie_get_idx(new_key);
                idx = new_idx;
                node = subnode;
            } while (old_idx == new_idx);

            subnode->cell[old_idx] = cell;
            subnode->cell[new_idx].key = key;
            subnode->cell[new_idx].val = val;
            ++ itrie->size;
            return 0;
        }
    }
}

int __itrie_update(itrie_t itrie, const cdata_t key, const cdata_t val) {

}


int __itrie_remove(itrie_t itrie, const cdata_t key, cdata_t *val) {

}


