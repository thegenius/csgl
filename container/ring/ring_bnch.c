#include <stdio.h>
#include <time.h>
#include "ring.h"

int bnch_ring_push_pull() {
    ring_t ring;
    ring_create(&ring, 0);
    const int num = 1000000;
    clock_t start = clock();
    clock_t push_finish;

    for (int i=0; i<num; ++i) {
        ring_push(ring, i);
    }

    push_finish = clock();

    cdata_t val;
    for (int i=0; i<num; ++i) {
        ring_pull(ring, &val);
    }


    clock_t finish = clock();
    printf("clocks per sec: %d\n", CLOCKS_PER_SEC);
    printf("%d push time used: %d clocks = %.2f s\n", num, push_finish - start, (double)(push_finish - start)/CLOCKS_PER_SEC);
    printf("%.2f push per second\n",  (double)num * CLOCKS_PER_SEC / (push_finish - start));

    printf("%d delete time used: %d clocks = %.2f s\n", num, finish - push_finish, (double)(finish - push_finish)/CLOCKS_PER_SEC);
    printf("%.2f delete per second\n",  (double)num * CLOCKS_PER_SEC / (finish - push_finish));
    ring_delete(&ring);
    return 1;
}

int main(int argc, char *argv[]) {
    bnch_ring_push_pull();

    return 0;
}
