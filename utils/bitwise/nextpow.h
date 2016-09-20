#ifndef __NEXTPOW_H__
#define __NEXTPOW_H__
#include <stdint.h>
extern inline uint32_t next_pow_u32(uint32_t value) {
	size_t n = value - 1;
	n = n | (n>>1);
	n = n | (n>>2);
	n = n | (n>>4);
	n = n | (n>>8);
	n = n | (n>>16);
	return n + 1;
}
extern inline uint64_t next_pow_u64(uint64_t value) {
	size_t n = value - 1;
	n = n | (n>>1);
	n = n | (n>>2);
	n = n | (n>>4);
	n = n | (n>>8);
	n = n | (n>>16);
	n = n | (n>>32);
	return n + 1;
}
#endif
