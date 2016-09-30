#ifndef __RING_STRUCT_H__
#define __RING_STRUCT_H__

struct ring {
	size_t head;
	size_t tail;
	size_t size;
	size_t capc;
	cdata_t *data;
};

#endif
