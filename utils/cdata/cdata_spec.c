#include <stdio.h>
#include <assert.h>
#include "cdata.h"

int cmp_int_func(cdata_t a, cdata_t b) {
    return a.i - b.i;
}

int verify_cdata() {
    cdata_t val = {23};
    assert(val.i == 23);    

    cdata_t arr[] = { {1}, {2}, {3}, {4}, {5} };
    int len = sizeof(arr)/sizeof(cdata_t);
    for (int i=0; i<len; ++i) {
        assert(arr[i].i == i+1);
    }

    for (int i=0; i<len-1; ++i) {
        int cmp = cmp_int_func(arr[i], arr[i+1]);
        assert (cmp == -1);
    }
    return 1;
}

int main(int argc, char* argv[]) {

    assert(verify_cdata());
    puts("verify success, satisfy the specification!");
    return 0;
}
