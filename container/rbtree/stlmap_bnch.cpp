#include <map>
#include <string>
#include <iostream>
#include <cstdio>

using namespace std;


int bnch_rbtree_insert_delete() {
    map<int, int> mapStudent;
    const int num = 3000000;
    clock_t start = clock();
    int sum = 0;
    for (int i=0; i<num; ++i) {
        mapStudent.insert(pair<int, int>(i, i));
    }

    clock_t insert_finish = clock();

    map<int, int>::iterator  iter;
    for(iter = mapStudent.begin(); iter != mapStudent.end();) {
        sum += iter->first;
        mapStudent.erase(iter++);
    }

    clock_t finish = clock();
    printf("clocks per sec: %d\n", CLOCKS_PER_SEC);
    printf("%d ops time used: %d clocks = %.2f s\n", 2*num, finish - start, (double)(finish - start)/CLOCKS_PER_SEC);
    printf("%.2f ops per second\n",  (double)2*num * CLOCKS_PER_SEC / (finish - start));

    printf("%d insert time used: %d clocks = %.2f s\n", num, insert_finish - start, (double)(insert_finish - start)/CLOCKS_PER_SEC);
    printf("%.2f insert per second\n",  (double)num * CLOCKS_PER_SEC / (insert_finish - start));
    printf("%d delete time used: %d clocks = %.2f s\n", num, finish - insert_finish, (double)(finish - insert_finish)/CLOCKS_PER_SEC);
    printf("%.2f delete per second\n",  (double)num * CLOCKS_PER_SEC / (finish - insert_finish));

    printf("sum :%d\n", sum);

    return 1;
}

int main() {
    bnch_rbtree_insert_delete();
    return 0;
}

