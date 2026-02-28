#include "Tile.h"

vec2 Tile::GetPos()
{
	return inR->GetPos();
}

int Tile::GetState()
{
	return state;
}

void Tile::SetState(int s)
{
	state = s;
}

Tile::Tile(float x, float y, float dx, float dy, int s)
{
	inR = new Rect(x, y, dx*0.8, dy*0.8);
	outR = new Rect(x, y, dx, dy);
	state = s;
}

void Tile::Draw(Brush* outB, Brush* inB0, Brush* inB, bool inTexMode)
{
	outB->DrawRect(outR, inTexMode);
	inB0->DrawRect(inR, inTexMode);
	if(state > 0)
		inB->DrawRect(inR, inTexMode);
}
