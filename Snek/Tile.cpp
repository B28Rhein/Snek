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

void Tile::Draw(Brush outB, Brush inB)
{
	outB.DrawRect(outR);
	inB.DrawRect(inR);
}
