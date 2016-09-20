#ifndef __MURMURHASH3_H__
#define __MURMURHASH3_H__
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

/*********************************** public api ***********************************************************/
void murmur_hash3_32 (const void *key, const size_t len, const uint32_t seed, uint32_t *hash);
void murmur_hash3_128 (const void *key, const size_t len, const uint32_t seed, uint64_t *hash1, uint64_t *hash2);
/**********************************************************************************************************/

static inline uint32_t rol_u32 (uint32_t x, int8_t r) {
  return (x << r) | (x >> (32 - r));
}

static inline uint32_t ror_u32 (uint32_t x, int8_t r) {
  return (x >> r) | (x << (32 - r));
}

static inline uint64_t rol_u64 (uint64_t x, int8_t r) {
  return (x << r) | (x >> (64 - r));
}

static inline uint64_t ror_u64 (uint64_t x, int8_t r) {
  return (x >> r) | (x << (64 - r));
}

static inline uint32_t fmix_u32 (uint32_t v) {
	v ^= v >> 16;
	v *= 0x85ebca6b;
	v ^= v >> 13;
	v *= 0xc2b2ae35;
	v ^= v >> 16;
	return v;
}

static inline uint64_t fmix_u64 (uint64_t k) {
	k ^= k >> 33;
	k *= 0xff51afd7ed558ccd;
	k ^= k >> 33;
	k *= 0xc4ceb9fe1a85ec53;
	k ^= k >> 33;
	return k;
}

void murmur_hash3_32 (const void * key, const size_t len, const uint32_t seed, uint32_t *hash) {
	const uint8_t * data = (const uint8_t*)key;
	const int nblocks = len >> 2;

	uint32_t h1 = seed;
	uint32_t c1 = 0xcc9e2d51;
	uint32_t c2 = 0x1b873593;

	/* process body */
	int i;
	const uint32_t * body = (const uint32_t *)(data);
	for (i=0; i<nblocks; ++i) {
		uint32_t k1 = body[i];

		k1 *= c1;
		k1 = rol_u32(k1, 15);
		k1 *= c2;
    
		h1 ^= k1;
		h1 = rol_u32(h1, 13); 
		h1 = h1*5 + 0xe6546b64;
	}

	/* process tail */
	const uint8_t * tail = (const uint8_t*)(data + (nblocks<<2));
	uint32_t k1 = 0;
	switch(len & 3) {
		case 3: k1 ^= tail[2] << 16;
		case 2: k1 ^= tail[1] << 8;
		case 1: k1 ^= tail[0];
		k1 *= c1; 
		k1 = rol_u32(k1, 15); 
		k1 *= c2; 
		h1 ^= k1;
  };

	/* final process */
	h1 ^= len;
	h1 = fmix_u32(h1);
	*hash = h1;
}

void murmur_hash3_128 (const void* key, const size_t len, const uint32_t seed, uint64_t* hash_1, uint64_t* hash_2) {
	const uint8_t * data = (const uint8_t*)key;
	const int nblocks = len >> 4;

	uint64_t h1 = seed;
	uint64_t h2 = seed;
	uint64_t c1 = 0x87c37b91114253d5;
	uint64_t c2 = 0x4cf5ad432745937f;

	/* process body, 128-bit per unit */
	int i;
	const uint64_t * body = (const uint64_t *)(data);
	for (i=0; i<nblocks; ++i) {
		uint64_t k1 = body[i*2 + 0];
		uint64_t k2 = body[i*2 + 1];

		k1 *= c1; 
		k1 = rol_u64(k1, 31); 
		k1 *= c2; 
		h1 ^= k1;
		
		h1 = rol_u64(h1, 27); 
		h1 += h2; 
		h1 = h1*5 + 0x52dce729;

		k2 *= c2; 
		k2 = rol_u64(k2,33); 
		k2 *= c1; 
		h2 ^= k2;

		h2 = rol_u64(h2,31); 
		h2 += h1; 
		h2 = h2*5 + 0x38495ab5;
	}

	/* process tail */
	const uint8_t * tail = (const uint8_t*)(data + (nblocks<<4));
	uint64_t k1 = 0;
	uint64_t k2 = 0;
	switch (len & 15) {
		case 15: k2 ^= (uint64_t)(tail[14]) << 48;
		case 14: k2 ^= (uint64_t)(tail[13]) << 40;
		case 13: k2 ^= (uint64_t)(tail[12]) << 32;
		case 12: k2 ^= (uint64_t)(tail[11]) << 24;
		case 11: k2 ^= (uint64_t)(tail[10]) << 16;
		case 10: k2 ^= (uint64_t)(tail[ 9]) << 8;
		case  9: k2 ^= (uint64_t)(tail[ 8]) << 0;
		k2 *= c2; k2  = rol_u64(k2, 33); k2 *= c1; h2 ^= k2;

		case  8: k1 ^= (uint64_t)(tail[ 7]) << 56;
		case  7: k1 ^= (uint64_t)(tail[ 6]) << 48;
		case  6: k1 ^= (uint64_t)(tail[ 5]) << 40;
		case  5: k1 ^= (uint64_t)(tail[ 4]) << 32;
		case  4: k1 ^= (uint64_t)(tail[ 3]) << 24;
		case  3: k1 ^= (uint64_t)(tail[ 2]) << 16;
		case  2: k1 ^= (uint64_t)(tail[ 1]) << 8;
		case  1: k1 ^= (uint64_t)(tail[ 0]) << 0;
		k1 *= c1; 
		k1  = rol_u64(k1, 31); 
		k1 *= c2; 
		h1 ^= k1;
	};

	/* final process */
	h1 ^= len; 
	h2 ^= len;

	h1 += h2;
	h2 += h1;

	h1 = fmix_u64(h1);
	h2 = fmix_u64(h2);

	h1 += h2;
	h2 += h1;

	*hash_1 = h1;
	*hash_2 = h2;
}

#ifdef __cplusplus
}
#endif

#endif /* __MURMURHASH3_H__ */
