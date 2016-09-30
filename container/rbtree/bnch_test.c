#include <stdio.h>
#include <time.h>

struct a_t{
    int left;
    int right;
};

struct b_t{
    int arr[2];
};


int x;
struct b_t b_global;

int func_x() {
    int t = 0;

    if (t) {
        printf("%d\n", 125);
    }

    if (t == x) {
        printf("%d\n", 126);
    }
}


int func_y1() {
    int y;
    if (x>0) {
        y = b_global.arr[1];
    } else {
        y = b_global.arr[0];
    }
    return y;
}

int func_y2() {
    return b_global.arr[x>0];
}







int func_a() {
    struct a_t a = {1,2};
    struct b_t b = { {1,2}};

    int sum_a = 123;
    for (int i=0; i<100; ++i) {
        sum_a += a.left;
        sum_a += a.right;
    }
    return 0;
} 

int func_b() {
    struct a_t a = {1,2};
    struct b_t b = { {1,2}};

    int sum_b = 124;
    for (int i=0; i<100; ++i) {
        sum_b += b.arr[0];
        sum_b += b.arr[1];
    }

    return 0;
}


int main() {

    struct a_t a = {1,2};
    struct b_t b = { {1,2}};

    int sum_a = 0;
    for (int i=0; i<100; ++i) {
        sum_a += a.left;
        sum_a += a.right;
    }
    printf("%d\n", sum_a);
    int sum_b = 0;
    for (int i=0; i<100; ++i) {
        sum_b += b.arr[0];
        sum_b += b.arr[1];
    }
    printf("%d\n", sum_b);

    const int num = 180000000;
    clock_t start, finish;

    start = clock();
    for (int i=0; i<num; ++i) {
        sum_b += b.arr[0];
        sum_b += b.arr[1];
    }
    finish = clock();
    printf("sum_b:%d time used:%d \n", sum_b, finish - start);


    start = clock();
    for (int i=0; i<num; ++i) {
        sum_a += a.left;
        sum_a += a.right;
    }
    finish = clock();
    printf("sum_a:%d time used:%d \n", sum_a, finish - start);


    start = clock();
    for (int i=0; i<num; ++i) {
        sum_b += func_y2();
    }
    finish = clock();
    printf("sum_b:%d time used:%d \n", sum_b, finish - start);

    start = clock();
    for (int i=0; i<num; ++i) {
        sum_a += func_y1();
    }
    finish = clock();
    printf("sum_a:%d time used:%d \n", sum_a, finish - start);

    start = clock();
    for (int i=0; i<num; ++i) {
        sum_b += func_y1();
    }
    finish = clock();
    printf("sum_b:%d time used:%d \n", sum_b, finish - start);

    start = clock();
    for (int i=0; i<num; ++i) {
        sum_a += func_y2();
    }
    finish = clock();
    printf("sum_a:%d time used:%d \n", sum_a, finish - start);










    return 0;
}
