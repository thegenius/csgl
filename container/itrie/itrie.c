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
        if (itrie_has_sub(node->cell[i].key)) {
            itrie_delete_iter(node->cell[i].val.ptr);
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

int __itrie_iter_search(itrie_t itrie, cdata_t key, itrie_iter_t iter) {
    itrie_node_t node = itrie->root;
    itrie_iter_clear(iter);
    for (int i=0; i<itrie->max_level; ++i) {
        size_t idx = itrie_get_idx(key);
        itrie_pair_t cell = node->cell[idx];
        itrie_iter_push(iter, node, idx);
        if (itrie_has_sub(cell.key)) {
            node = cell.val.ptr;
            key = (cdata_t)((uintptr_t)key.ptr >> ITRIE_NODE_IDX_BITS);
        } else {
            if (!itrie_has_val(cell.key)) {
                itrie_iter_clear(iter);
                return -1;
            }

            key = (cdata_t)((uintptr_t)key.ptr >> ITRIE_NODE_IDX_BITS);
            if (itrie_decode_val(cell.key).ptr == key.ptr) {
                iter->pre = (cdata_t)(iter->pre.uintptr | (key.uintptr << ((iter->cur+1) << 2)));
                return 0;
            }

            itrie_iter_clear(iter);
            return -1;
        }
    }
    itrie_iter_clear(iter);
    return -1;
}

int __itrie_elem_insert(itrie_t itrie, cdata_t key, const cdata_t val) {
    //iprintf("- insert - key:%llx val:%d\n", key, val);
    size_t idx, old_idx, new_idx;
    itrie_node_t node = itrie->root;
    for (int i=0; i<itrie->max_level; ++i) {
        idx = itrie_get_idx(key);
        itrie_pair_t cell = node->cell[idx];
        if (itrie_has_sub(cell.key)) {
            //printf("- insert - level:%d has subnode in cell[%d]\n", i, idx);
            node = cell.val.ptr;
            key = (cdata_t)((uintptr_t)key.ptr >> ITRIE_NODE_IDX_BITS);
            continue;
        }

        //printf("- insert - level:%d no subnode\n", i);
        key = (cdata_t)((uintptr_t)key.ptr >> ITRIE_NODE_IDX_BITS);
        if (!itrie_has_val(cell.key)) {
            //printf("- insert - level:%d has no val, just insert into empty cell \n", i);
            node->cell[idx].key = itrie_encode_val(key);
            assert(itrie_has_val(node->cell[idx].key));
            assert(!itrie_has_sub(node->cell[idx].key));
            node->cell[idx].val = val;
            ++ itrie->size;
            return 0;
        }

        cdata_t old_key = itrie_decode_val(cell.key);
        cdata_t new_key = key;
        if (old_key.ptr == new_key.ptr) {
            //printf("- insert - level:%d has val with same key:%llx \n", i, key.ptr);
            return -1;
        }

        //printf("- insert - level:%d new node\n", i);
        old_idx = itrie_get_idx(old_key);
        new_idx = itrie_get_idx(new_key);
        itrie_node_t subnode;
        int level = i;
        while (old_idx == new_idx) {
            //printf("- insert - level:%d, old_idx == new_idx : %llu == %llu, new subnode\n", level, old_idx, new_idx);
            itrie_node_create(&subnode);
            node->cell[idx].key = (cdata_t)HAS_SUB_NODE;
            node->cell[idx].val.ptr = subnode;
            //printf("- insert - level:%d replace cell[%d] with %llx\n", level, idx, subnode);
            idx = old_idx;
            old_key = (cdata_t)((uintptr_t)old_key.ptr >> ITRIE_NODE_IDX_BITS);
            new_key = (cdata_t)((uintptr_t)new_key.ptr >> ITRIE_NODE_IDX_BITS);
            old_idx = itrie_get_idx(old_key);
            new_idx = itrie_get_idx(new_key);
            node = subnode;
            ++ level;
        };

        //printf("- insert - level:%d, old_idx != new_idx : %llu != %llu, new subnode\n", level, old_idx, new_idx);
        itrie_node_create(&subnode);
        node->cell[idx].key = (cdata_t)HAS_SUB_NODE;
        node->cell[idx].val.ptr = subnode;
        //printf("- insert - level:%d replace cell[%d] with %llx\n", level, idx, subnode);

        subnode->cell[old_idx].key = itrie_encode_val((cdata_t)(old_key.uintptr >> ITRIE_NODE_IDX_BITS));
        subnode->cell[old_idx].val = cell.val;
        //printf("key:%llx val:%d\n", subnode->cell[old_idx].key, subnode->cell[old_idx].val);


        subnode->cell[new_idx].key = itrie_encode_val((cdata_t)(new_key.uintptr >> ITRIE_NODE_IDX_BITS));
        subnode->cell[new_idx].val = val;
        //printf("key:%llx val:%d\n", subnode->cell[new_idx].key, subnode->cell[new_idx].val);
        ++ itrie->size;
        return 0;
    }
    return -1;
}

int __itrie_elem_update(itrie_t itrie, const cdata_t key, const cdata_t val) {

}


int __itrie_elem_remove(itrie_t itrie, cdata_t key, cdata_t *val) {
    itrie_node_t node = itrie->root;
    for (int i=0; i<itrie->max_level; ++i) {
        size_t idx = itrie_get_idx(key);
        itrie_pair_t cell = node->cell[idx];
        if (itrie_has_sub(cell.key)) {
            node = cell.val.ptr;
            key = (cdata_t)((uintptr_t)key.ptr >> ITRIE_NODE_IDX_BITS);
        } else {
            if (!itrie_has_val(cell.key)) {
                return -1;
            }

            key = (cdata_t)((uintptr_t)key.ptr >> ITRIE_NODE_IDX_BITS);
            if (itrie_decode_val(cell.key).ptr == key.ptr) {
                *val = cell.val;
                node->cell[idx].key.ptr = 0;
                return 0;
            }

            return -1;
        }
    }
    return -1;
}


