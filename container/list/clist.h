
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>
typedef union clist_data_t {
	short s;
	int i;
	long l;
	long long ll;
	unsigned short us;
	unsigned int ui;
	unsigned long ul;
	unsigned long long ull;
	size_t sz;
	int8_t i8;
	int16_t i16;
	int32_t i32;
	int64_t i64;
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;
	uint64_t u64;
	float f;
	double d;
	bool b;
	char c8;
	char16_t c16;
	char32_t c32;
	void* ptr;
} clist_data_t;

struct clist_node_t {
	struct clist_node_t *prev;
	struct clist_node_t *next;
	union clist_data_t data;
};

typedef struct clist_node_t clist_t;
typedef struct clist_node_t clist_node_t;

/*
int clist_insert(const clist_t *clist, const size_t index,const clist_data_t value);
int clist_delete(const clist_t *clist, const size_t index);
int clist_remove(const clist_t *clist, const clist_data_t value);
int clist_reverse(const clist_t *clist);
int clist_sort(const clist_t *clist);
*/

clist_t* clist_create();
int clist_destroy(clist_t** clist);
size_t clist_size(const clist_t* clist);

int clist_push_back(clist_t *clist, const clist_data_t value);
clist_data_t clist_pop_front(clist_t *clist);

int clist_index(const clist_t *clist, const size_t index);
int clist_find(const clist_t *clist, const clist_data_t value);

int clist_clear(const clist_t *clist);
int clist_merge(const clist_t *clist_a,const clist_t *clist_b);

inline clist_node_t* clist_node_create() {
	clist_node_t* node = (clist_node_t*)malloc(sizeof(clist_node_t));
	return node;
}

inline int clist_node_destroy(clist_node_t ** elem) {
	free(*elem);
	*elem = 0;
	return 0;
}

inline clist_t* clist_create() {
	clist_t* clist = (clist_t*)calloc(1, sizeof(clist_t));
	clist->prev = clist;
	clist->next = clist;
	return clist;
}

int clist_destroy(clist_t** clist) {
	clist_node_t *node = *clist;
	node->prev->next = 0;
	while (node) {
		clist_node_t *destroy_node = node;
		node = node->next;
		clist_node_destroy(&destroy_node);
	}
	*clist = 0;
	return 0;
}

inline size_t clist_size(const clist_t* clist) {
	return clist->data.sz;
}

inline clist_node_t* clist_head(const clist_t* clist) {
	return clist->next;
}

inline clist_node_t* clist_tail(const clist_t* clist) {
	return clist->prev;
}

#define clist_push_back(clist, data) __clist_push_back(clist, (clist_data_t)data)
inline int __clist_push_back(clist_t* clist, const clist_data_t data) {
	clist_node_t *elem = clist_node_create();
	clist_node_t *node = clist_tail(clist);	
	elem->data = data;
	elem->prev = node;
	elem->next = node->next;
	node->next->prev = elem;
	node->next = elem;
	++ clist->data.sz;
	return 0;
}

inline clist_data_t clist_pop_front(clist_t* clist) {
	clist_node_t *node = clist_head(clist);
	clist_data_t data = node->data;
	node->prev->next = node->next;
	node->next->prev = node->prev;
	clist_node_destroy(&node);
	-- clist->data.sz;
	return data;
}






