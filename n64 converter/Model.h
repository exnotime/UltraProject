#pragma once
#include <stdint.h>
struct ModelInputData {
	float scale; //since the n64 stores vertex position in s16 it might be a good idea to scale the model so vertices dont all collapse into a single point
};

enum VertexType : uint8_t {
	COLORS,
	NORMALS
};

//Vertices on the n64 can either have vertex colors or normals
struct UltraVertex_tn {
	short pos[3]; //signed 16 bit
	unsigned short flags; //unused :)
	short uv[2]; //s10.5 format
	char normal[3]; //-128-127 signed 8 bit
	unsigned char alpha;
};

struct UltraVertex_tc {
	short pos[3]; //signed 16 bit
	unsigned short flags; //unused :)
	short uv[2]; //s10.5 format
	unsigned char color[3]; //rgb
	unsigned char alpha;
};

typedef union {
	UltraVertex_tn vtx_tn;
	UltraVertex_tc vtx_tc;
	long long int alignment;
} UltraVertex;

enum IndexType : uint8_t {
	U8,
	U16
};

struct ModelHeader {
	uint16_t magic = 0xFADE;
	uint16_t vertexCount;
	uint8_t meshCount;
	uint8_t textureCount;
	uint16_t padding = 0xDDDD;
};

struct MeshHeader {
	uint16_t magic = 0xDEAD;
	uint16_t indexCount;
	uint16_t indexOffset;
	uint8_t texture;
	uint8_t vertexType;
};

void ConvertModel(const char* file, const char* outputFile, ModelInputData data);