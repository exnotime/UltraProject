#include "Model.h"
#include "Utility.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <vector>

void ConvertModel(const char* file, const char* outputFile, ModelInputData data) {
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(file, aiProcess_CalcTangentSpace | aiProcess_FlipUVs
		| aiProcess_GenUVCoords | aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);


	if (!scene) {
		return;
	}
	std::vector<MeshHeader> meshes;
	std::vector<UltraVertex> vertices;
	std::vector<uint16_t> indices;
	//meshes
	if (scene->HasMeshes()) {
		uint32_t meshCount = scene->mNumMeshes;

		uint32_t indexOffset = 0;
		for (uint32_t m = 0; m < meshCount; ++m) {
			MeshHeader meshHeader;
			aiMesh* mesh = scene->mMeshes[m];
			meshHeader.vertexCount = mesh->mNumVertices;

			for (uint32_t v = 0; v < meshHeader.vertexCount; ++v) {
				UltraVertex vertex;
				vertex.vtx_tc.pos[0] = EndianSwap((short)mesh->mVertices[v].x);
				vertex.vtx_tc.pos[1] = EndianSwap((short)mesh->mVertices[v].y);
				vertex.vtx_tc.pos[2] = EndianSwap((short)mesh->mVertices[v].z);

				vertex.vtx_tc.uv[0] = EndianSwap(FloatToFixedS_10_5(mesh->mTextureCoords[0][v].x));
				vertex.vtx_tc.uv[1] = EndianSwap(FloatToFixedS_10_5(mesh->mTextureCoords[0][v].y));

				if (mesh->HasVertexColors(0)) {
					vertex.vtx_tc.color[0] = mesh->mColors[0][v].r * 255;
					vertex.vtx_tc.color[1] = mesh->mColors[0][v].g * 255;
					vertex.vtx_tc.color[2] = mesh->mColors[0][v].b * 255;
					meshHeader.vertexType = COLORS;
				}
				else if (mesh->HasNormals()) {
					vertex.vtx_tn.normal[0] = (int8_t)(mesh->mNormals[v].x * 128);
					vertex.vtx_tn.normal[1] = (int8_t)(mesh->mNormals[v].y * 128);
					vertex.vtx_tn.normal[2] = (int8_t)(mesh->mNormals[v].z * 128);
					meshHeader.vertexType = NORMALS;
				}
				vertex.vtx_tc.alpha = 255;
				vertices.push_back(vertex);
			}
			meshHeader.texture = mesh->mMaterialIndex;
			for (uint32_t f = 0; f < mesh->mNumFaces; ++f) {
				indices.push_back(indexOffset + mesh->mFaces[f].mIndices[0]);
				indices.push_back(indexOffset + mesh->mFaces[f].mIndices[1]);
				indices.push_back(indexOffset + mesh->mFaces[f].mIndices[2]);
			}
			indexOffset += meshHeader.vertexCount;
			meshes.push_back(meshHeader);
		}
	}
	std::vector<UltraVertex> outVertices;
	for (uint32_t i = 0; i < indices.size(); i += 3){
		outVertices.push_back(vertices[indices[i + 0]]);
		outVertices.push_back(vertices[indices[i + 1]]);
		outVertices.push_back(vertices[indices[i + 2]]);
	}

	ModelHeader modelHeader;
	modelHeader.meshCount = meshes.size();
	modelHeader.vertexCount = outVertices.size();

	//write file
	FILE* fout = fopen(outputFile, "wb");
	if (fout) {
		fwrite(&modelHeader, sizeof(uint8_t), sizeof(ModelHeader), fout);
		fwrite(meshes.data(), sizeof(uint8_t), sizeof(MeshHeader) * meshes.size(), fout);
		fwrite(outVertices.data(), sizeof(uint8_t), sizeof(UltraVertex) * outVertices.size(), fout);
	}
}
