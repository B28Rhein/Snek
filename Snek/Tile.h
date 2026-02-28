#pragma once
#include "Defs.h"
#include "Rect.h"
class Tile
{
	Rect* inR;
	Rect* outR;
	int state;
public:
	vec2 GetPos();
	int GetState();
	void SetState(int s);
	Tile(float, float, float, float, int);
	void Draw(Brush* outB, Brush* inB0, Brush* inB, bool inTexMode);
};

