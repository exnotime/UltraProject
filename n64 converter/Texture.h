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

struct TextureHeader{
	uint16_t magic = 0xD00D;
	uint16_t format;
	uint16_t width;
	uint16_t height;
};

struct TextureInputData {
	TextureFormat format;
};

void ConvertTexture(const char* filename, const char* outFile, TextureFormat targetFormat);