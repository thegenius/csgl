#include "treearray.h"
#include <stdint.h>
#include <stdlib.h>

struct treearray_t {
	size_t size;
	size_t data[0];
};

size_t treearray_at(treearray_t *arr, size_t i) {
	return arr->data[i];
}

static inline size_t lowbit(size_t x) {
	return x & (-x);
}

int treearray_create(treearray_t **arr, const size_t size) {
	(*arr) = (treearray_t*)calloc(1, sizeof(treearray_t) +  (size+1) * sizeof(size_t));
	(*arr)->size = size;
	return 0;
}

int treearray_delete(treearray_t **arr) {
	free(*arr);
	*arr = 0;
	return 0;
}

int treearray_update(treearray_t *arr, size_t idx, const int val) {
	++ idx;
	while (idx <= arr->size) {
		arr->data[idx] += val;
		idx += lowbit(idx);
	}
	return 0;
}

int treearray_length(const treearray_t *arr, size_t idx) {
	if (idx >= arr->size) {
		idx = arr->size - 1;
	}
	++ idx;
	int result = 0;
	while (idx > 0) {
		result += arr->data[idx];
		idx -= lowbit(idx);
	}
	return result;
}

int treearray_locate(const treearray_t *arr, const size_t loc) {
	size_t head = 0;
	size_t tail = arr->size - 1;
	while (head < tail) {
		size_t mid = head + ((tail - head) >> 1);
		size_t val = treearray_length(arr, mid);
		if (val < loc) {
			head = mid + 1;
		} else {
			tail = mid;
		}
	}
	return head;
}







