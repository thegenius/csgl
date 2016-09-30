#include <string.h>
#include <stdio.h>
#include "ring.h"
#include "ring_struct.h"

static inline size_t next_power(size_t size) {
    printf("origin size: %d\n", size);
	size_t n = size - 1;
	n = n | (n>>1);
	n = n | (n>>2);
	n = n | (n>>4);
	n = n | (n>>8);
	n = n | (n>>16);
	if (sizeof(size_t) == 8) {
		n = n | (n>>32);
	}
    printf("power size: %d\n", n+1);
	return n+1;
}

int ring_create(ring_t *ring, size_t size) {
    *ring = (struct ring*)calloc(1, sizeof(struct ring));
    if (*ring == 0) {
        return -1;
    }

    size_t data_size = next_power(size + 1);
    printf("src size:%d create size :%d\n", size, data_size);
    (*ring)->data = (cdata_t*)calloc(1, data_size * sizeof(cdata_t));
    printf("data addr:%llx\n", (*ring)->data);
    if ((*ring)->data == 0) {
        return -1;
    }

    (*ring)->capc = data_size - 1;
    (*ring)->head = 0;
    (*ring)->tail = 0;
    (*ring)->size = 0;
    return 0;
}

int ring_realloc(ring_t ring, size_t size) {
    puts("realloc");
    if (size <= ring->capc) {
        return 0;
    }
    size_t data_size = next_power(size + 1);
    printf("old size:%d\n", (ring->capc + 1) * sizeof(cdata_t));
    printf("old ptr:%llx new size:%d\n", ring->data, data_size * sizeof(cdata_t));
    ring->data = (cdata_t*)realloc(ring->data, data_size * sizeof(cdata_t));
    if (!ring->data) {
        return -1;
    }
    printf("new data addr:%llx\n", ring->data);
    if (ring->tail < ring->head) {
        size_t offset = (data_size - ring->capc - 1) * sizeof(cdata_t);
        int head = ring->head;
        void *src = &(ring->data[head]);
        void *dst = src + offset;
        memmove(dst, src, offset);
    }
    ring->capc = data_size - 1;
    return 0;
}


int ring_delete(ring_t *ring) {
	free(*ring);
	*ring = 0;
	return 0;
}

int __ring_push(ring_t ring, const cdata_t data) {
	size_t head = ring->head;
	size_t tail = ring->tail;
	size_t next_tail = (tail+1) & ring->capc;
	if (next_tail == head) {
        int err = ring_realloc(ring, 2*ring->capc);
        if (err) {
		    return -1;
        }
	}
	ring->data[tail] = data;
	ring->tail = next_tail;
	++ ring->size;
	return 0;
}

int __ring_pull(ring_t ring, cdata_t *data) {
	if (ring->size <= 0) {
		return -1;
	}
	size_t head = ring->head;
	size_t next_head = (head+1) & ring->capc;
	*data = ring->data[head];
	ring->head = next_head;
	-- ring->size;
	return 0;
}

bool ring_empty(ring_t ring) {
	return ring->head == ring->tail;
}

bool ring_full(ring_t ring) {
	return (ring->head & ring->capc) == ((ring->tail+1) & ring->capc);
}


