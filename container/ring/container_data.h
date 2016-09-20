#ifndef __CONTAINER_DATA_H__
#define __CONTAINER_DATA_H__
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>
union container_data_t {
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
};
typedef union container_data_t container_data_t;

#endif
