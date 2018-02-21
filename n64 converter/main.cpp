//The purpose of this program is to convert some common resource formats to n64 compatible resources
#include "Texture.h"
#include "Model.h"
#include <stdio.h>
#include <string.h>
#include "Utility.h"
enum ConvertionType {
	TEXTURE,
	MODEL,
	SOUND,
	MUSIC
};

struct AppOptions {
	ConvertionType type;
	char* input;
	char* output;
	union {
		TextureInputData texData;
		ModelInputData modelData;
	} data;
};

void PrintHelp() {
	printf("This is the help text\n");
}

AppOptions ParseArguments(int argc, char** argv) {
	AppOptions opt;
	for (int i = 0; i < argc; ++i) {
		if (strcmp(argv[i], "-h") == 0) {
			PrintHelp();
		}
	}
	return opt;
}

int main(int argc, char** argv){
	//ConvertTexture("z_tex.bmp", "z_tex.utx", RGBA8);
	ModelInputData inputData;
	inputData.scale = 1.0f;
	ConvertModel("teapot/optimizedTeapot.obj", "teapot.umf", inputData);
}