#pragma once
#include <stdint.h>
#include <math.h>
#include "fixedp.h"

static uint16_t EndianSwap(uint16_t i) {
	return (i >> 8 | i << 8);
}

static int16_t EndianSwap(int16_t i) {
	return (i >> 8 | i << 8);
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