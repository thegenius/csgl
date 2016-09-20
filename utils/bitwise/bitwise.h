#ifndef __BITWISE_H__
#define __BITWISE_H__
#include <stdint.h>
#include "popcnt.h"
#include "nextpow.h"

extern size_t get_first_one_pos_u32(uint32_t container) {
	if (container == 0) {
		return -1;
	} else {
		return popcnt_u32(container ^ (container - 1)) - 1;
	}
}

extern size_t get_first_zero_pos_u32(uint32_t container) {
	return get_first_one_pos_u32(~container);
}

#endif
