#pragma once
#include <stdint.h>
#include <math.h>
#include "fixedp.h"

static uint16_t EndianSwap(uint16_t i) {
	return (i >> 8 | i << 8);
}

static int16_t EndianSwap(int16_t i) {
	return ((i >> 8) & 0x00ff) | ((i & 0x00ff) << 8);
}

static int16_t bswap16(int16_t value) {
	int8_t byte1, byte2;

	byte1 = value & 0xFF;
	byte2 = (value >> 8) & 0xFF;

	return byte1 << 8 | byte2;
}

static uint32_t EndianSwap(uint32_t i) {
	return	((i >> 24) & 0xff000) |
		((i << 8) & 0xff0000) |
		((i >> 8) & 0xff0000) |
		((i << 24) & 0xff000000);
}

static int16_t FloatToFixedS_10_5(float f) {
	return fixedp<true, 11, 5>(f).raw();
}