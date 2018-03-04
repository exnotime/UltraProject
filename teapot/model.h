#ifndef MODEL_H
#define MODEL_H
#include <nusys.h>

typedef struct{
	u16 magic;
	u16 vertexCount;
	u8 meshCount;
	u8 textureCount;
}ModelHeader;

typedef struct {
	u16 magic;
	u16 vertexCount;
	u16 vertexOffset;
	u8 texture;
	u8 vertexType;
}MeshHeader;

typedef struct {
	ModelHeader header;
	MeshHeader* meshes;
	Vtx* vertices;
}Model;

void LoadModel(u8* cartAddress, Model* model);

#endif
