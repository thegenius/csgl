#ifndef __CQUEUE_H__
#define __CQUEUE_H__
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>
#include "container_data.h"

typedef struct ring_t ring_t;

extern int ring_create(ring_t **ring, size_t size);
extern int ring_delete(ring_t **ring); 
extern int __ring_push(ring_t *ring, const container_data_t data);
extern int ring_pull(ring_t *ring, container_data_t *data);
extern bool ring_empty(ring_t *ring);
extern bool ring_full(ring_t *ring);

#define ring_push(node,data) __ring_push(node, (container_data_t)data)
#endif
