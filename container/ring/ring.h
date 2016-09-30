#ifndef __RING_H__
#define __RING_H__
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>
#include "cdata.h"

typedef struct ring_t ring_t;

extern int ring_create(ring_t **ring, size_t size);
extern int ring_delete(ring_t **ring); 
extern int __ring_push(ring_t *ring, const cdata_t data);
extern int ring_pull(ring_t *ring, cdata_t *data);
extern bool ring_empty(ring_t *ring);
extern bool ring_full(ring_t *ring);

#define ring_push(node,data) __ring_push(node, (cdata_t)(data))

#endif
