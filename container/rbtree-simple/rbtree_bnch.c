#include <stdio.h>
#include <time.h>
#include "rbtree.h"
extern size_t alloc_clocks;
extern size_t search_clocks;
extern size_t attach_clocks;
extern size_t balance_clocks;
extern size_t maintain_clocks;
extern size_t insert_maintain_clocks;

extern size_t delete_search_clocks;
extern size_t delete_maintain_clocks;


int bnch_rbtree_insert_delete() {
    rbtree_t *tree;
    rbtree_create(&tree, 0);
    const int num = 1000000;
    clock_t start = clock();
    clock_t insert_finish;

    for (int i=0; i<num; ++i) {
        rbtree_elem_insert(tree, i, i);
    }

    insert_finish = clock();

    cdata_t val;
    for (int i=0; i<num; ++i) {
        rbtree_elem_delete(tree, 0, &val);
    }


    clock_t finish = clock();
    printf("clocks per sec: %d\n", CLOCKS_PER_SEC);
    printf("%d insert time used: %d clocks = %.2f s\n", num, insert_finish - start - alloc_clocks, (double)(insert_finish - start - alloc_clocks)/CLOCKS_PER_SEC);
    printf("alloc clocks :%d\n", alloc_clocks);
    printf("search clocks :%d\n", search_clocks);
    printf("attach clocks :%d\n", attach_clocks);
    printf("balance clocks :%d\n", balance_clocks);
    printf("insert maintain clocks :%d\n", insert_maintain_clocks);

    printf("%.2f insert per second\n",  (double)num * CLOCKS_PER_SEC / (insert_finish - start));
    printf("%d delete time used: %d clocks = %.2f s\n", num, finish - insert_finish, (double)(finish - insert_finish)/CLOCKS_PER_SEC);
    printf("%.2f delete per second\n",  (double)num * CLOCKS_PER_SEC / (finish - insert_finish));
    printf("delete_maintain clocks :%d\n", delete_maintain_clocks);
    printf("delete_search clocks :%d\n", delete_search_clocks);
    rbtree_delete(&tree);
    return 1;
}

int main(int argc, char *argv[]) {
    bnch_rbtree_insert_delete();

    return 0;
}
