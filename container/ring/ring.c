#include "ring.h"

struct ring_t {
	size_t head;
	size_t tail;
	size_t size;
	size_t capc;
	cdata_t data[0];
};

extern inline size_t next_power(size_t size) {
	size_t n = size - 1;
	n = n | (n>>1);
	n = n | (n>>2);
	n = n | (n>>4);
	n = n | (n>>8);
	n = n | (n>>16);
	if (sizeof(size_t) == 8) {
		n = n | (n>>32);
	}
	return n+1;
}

int ring_create(ring_t **ring, size_t size) {
	size_t data_size = next_power(size);
	*ring = (ring_t*)calloc(1, sizeof(ring_t) + data_size * sizeof(cdata_t));
	(*ring)->capc = data_size - 1;
	(*ring)->head = 0;
	(*ring)->tail = 0; 
	(*ring)->size = 0; 
	return 0;
}

int ring_delete(ring_t **ring) {
	free(*ring);
	*ring = 0;
	return 0;
}

int __ring_push(ring_t *ring, const cdata_t data) {
	size_t head = ring->head;
	size_t tail = ring->tail;
	size_t next_tail = (tail+1) & ring->capc;
	if (next_tail == head) {
		return -1;
	}
	ring->data[tail] = data;
	ring->tail = next_tail;
	++ ring->size; 
	return 0;
}

int ring_pull(ring_t *ring, cdata_t *data) {
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

bool ring_empty(ring_t *ring) {
	return ring->head == ring->tail;
}

bool ring_full(ring_t *ring) {
	return (ring->head & ring->capc) == ((ring->tail+1) & ring->capc);
}


