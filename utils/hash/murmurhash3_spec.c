#include <stdio.h>
#include <stdint.h>
#include "murmurhash3.h"

int main(int argc, char* argv[]) {
	int data[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	uint32_t hash;
	murmur_hash3_32(data, sizeof(data), 0, &hash);
	printf("hash:%u\n", hash);

	uint64_t hash1, hash2;
	murmur_hash3_128(data, sizeof(data), 0, &hash1, &hash2);
	printf("hash1:%u\n", hash1);
	printf("hash2:%u\n", hash2);
	return 0;
}
