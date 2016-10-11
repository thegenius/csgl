#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "itrie.h"

int bnch_itrie_insert_delete() {
    itrie_t itrie;
    itrie_create(&itrie);
    const int num = 1000000;
    clock_t start = clock();
    clock_t insert_finish;

    for (int i=0; i<num; ++i) {
        itrie_elem_insert(itrie, i, i);
    }

    insert_finish = clock();

    cdata_t val;
    for (int i=0; i<num; ++i) {
        itrie_elem_remove(itrie, i, &val);
    }


    clock_t finish = clock();
    printf("clocks per sec: %d\n", CLOCKS_PER_SEC);
    printf("%d insert time used: %d clocks = %.2f s\n", num, insert_finish - start, (double)(insert_finish - start)/CLOCKS_PER_SEC);
    printf("%.2f insert per second\n",  (double)num * CLOCKS_PER_SEC / (insert_finish - start));

    printf("%d delete time used: %d clocks = %.2f s\n", num, finish - insert_finish, (double)(finish - insert_finish)/CLOCKS_PER_SEC);
    printf("%.2f delete per second\n",  (double)num * CLOCKS_PER_SEC / (finish - insert_finish));
    itrie_delete(&itrie);
    return 1;
}

int main(int argc, char *argv[]) {
    bnch_itrie_insert_delete();

    return 0;
}
