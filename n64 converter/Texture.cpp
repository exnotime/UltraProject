#include "Texture.h"
 #define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Utility.h"

#define RED_SHIFT 11
#define GREEN_SHIFT RED_SHIFT - 5
#define BLUE_SHIFT GREEN_SHIFT - 5

void RGBA5A1Conversion(uint8_t* dstData, uint8_t* srcData, int w, int h) {
	uint32_t dataOffset = 0;
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			uint16_t r, g, b, a;
			uint16_t result = 0;
			int pixel = y * w + x * 4;
			//5 bits per color channel 1 bit alpha
			r = srcData[pixel + 0] / 32;
			g = srcData[pixel + 1] / 32;
			b = srcData[pixel + 2] / 32;
			a = srcData[pixel + 3] > 0 ? 1 : 0;
			
			result |= (RED_SHIFT	<< r);
			result |= (GREEN_SHIFT	<< g);
			result |= (BLUE_SHIFT	<< b);
			result |= a;

			memcpy(dstData + dataOffset, &result, sizeof(uint16_t));
			dataOffset += sizeof(uint16_t);
		}
	}
}

void ConvertTexture(const char* filename, const char* outFile, TextureFormat targetFormat){
	int w, h, c;
	uint8_t* srcImage = (uint8_t*)stbi_load(filename, &w, &h, &c, 4);
	if(!srcImage)
		return;
	uint32_t size = (w * h * BPP[targetFormat]) / 8;
	uint8_t* dstImage = (uint8_t*)malloc(size);

	switch(targetFormat){
		case RGBA8:
			memcpy(dstImage, srcImage, size);// rgba8 textures dont need conversion
		break;
		case RGB5A1:
			RGBA5A1Conversion(dstImage, srcImage, w, h);
		break;
	};

	FILE* f = fopen(outFile, "wb");
	if (f) {
		//write header
		TextureHeader header;
		header.format = EndianSwap((uint16_t)targetFormat);
		header.width = EndianSwap((uint16_t)w);
		header.height = EndianSwap((uint16_t)h);
		fwrite(&header, sizeof(uint8_t), sizeof(TextureHeader), f);
		//write data
		fwrite(dstImage, sizeof(uint8_t), size, f);
		fclose(f);

	}
}