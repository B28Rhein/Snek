#pragma once
#include "Defs.h"
#include "Buffers.h"
class Rect
{
	float x;
	float y;
	float dx;
	float dy;
	
public:
	static Buffers* buf;
	Rect(float x, float y, float dx, float dy);
	Rect(Rect* rect);
	mat4 GetModel(float rotation = 0);
	std::pair<float*, float*> GetPosPtrs();
	vec2 GetPos();
	void SetPos(vec2 npos);
	void SetPos(float, float);
	uint GetVao();
};

