#ifndef ULTRA_TEXTURE_H
#define ULTRA_TEXTURE_H
typedef struct{
	u16 magic;
	u16 format;
	u16 width;
	u16 height;
} TextureHeader;
#endif

#ifdef C_PLUS_PLUS 
#pragma once
#include <stdint.h>
enum TextureFormat{
	RGBA8,
	RGB5A1,
	CI8,
	IA8,
	IA4,
	I3A1,
	I8,
	FORMAT_COUNT
};

//matches formats
static int BPP[FORMAT_COUNT] = {
	32,
	16,
	8,
	16,
	8,
	4,
	8
};

typedef struct TextureHeader{
	uint16_t magic;
	uint16_t format;
	uint16_t width;
	uint16_t height;
};

void ConvertTexture(const char* filename, const char* outFile, TextureFormat targetFormat);

#endif