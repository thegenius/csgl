#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "bsearch.h"

int cmp_func(const void *a, const void *b) {
    return ( *(int*)a - *(int*)b );
}

int verify_int() {
    int arr[] = {5, 20, 29, 32, 63};
    int len = sizeof(arr) / sizeof(int);
    for (int i=0; i<len; ++i) {
        int key = arr[i]; 
        int idx = binary_search(arr, len, sizeof(int), &key, cmp_func);
        assert( key == arr[idx] );
    }
    for (int i=0; i<len; ++i) {
        int key = arr[i] + 1; 
        int idx = binary_search(arr, len, sizeof(int), &key, cmp_func);
        assert(idx < 0);
        idx = - idx - 1;
        if (idx > 0) {
            assert(arr[idx-1] < key);
        }
        if (idx < len) {
            assert(arr[idx] > key);
        }
    }
    return 1;
}

int main(int argc, char *argv[]) {
    assert(verify_int());   
    return 0;
}
