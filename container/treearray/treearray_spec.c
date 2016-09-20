#include <stdio.h>
#include "treearray.h"


int verify_treearray() {
	treearray_t *arr;
	treearray_create(&arr, 10);

	treearray_update(arr, 0, 10);
	treearray_update(arr, 4, 5);

	for (int i=0; i<11; ++i) {
		printf("arr[%d] : %d\n", i, treearray_at(arr, i));
	}

	size_t sum = 0;
	for (int i=0; i<10; ++i) {
		sum = treearray_length(arr, i);
		printf("sum[%d] : %d\n", i, sum);
	}
	size_t loc = 0;
	for (int i=0; i<16; ++i) {
		loc = treearray_locate(arr, i);
		printf("loc[%d] : %d\n", i, loc);
	}

	treearray_delete(&arr);

	return 1;
}


int main(int argc, char* argv[]) {
	int success = verify_treearray();
	return 0;
}
