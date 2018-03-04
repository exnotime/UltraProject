#include "model.h"
void LoadModel(u8* cartAddress, Model* model){
	u32 fileOffset = 0;
	//header
	nuPiReadRom((u32)cartAddress, &model->header, sizeof(ModelHeader));
	fileOffset += sizeof(ModelHeader);
	//meshes
	model->meshes = (MeshHeader*)malloc(sizeof(MeshHeader) * model->header.meshCount);
	nuPiReadRom((u32)cartAddress + fileOffset, model->meshes, sizeof(MeshHeader) * model->header.meshCount);
	fileOffset += sizeof(MeshHeader) * model->header.meshCount;
	//vertices
	model->vertices = (Vtx*)malloc(sizeof(Vtx) * model->header.vertexCount);
	nuPiReadRom((u32)cartAddress + fileOffset, model->vertices, sizeof(Vtx) * model->header.vertexCount);
}