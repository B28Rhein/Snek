#pragma once
#include "Defs.h"
class Buffers
{
	uint VAO = 0;
	uint VBO = 0;
	uint EBO = 0;
	uint TexBuf = 0;
public:
	Buffers(float vertices[], uint indices[], int vertCount, int indiceCount);
	Buffers(float vertices[], float textureVerts[], uint indices[], int vertCount, int indiceCount);
	uint GetVao();
	uint GetTexBuf();
};

