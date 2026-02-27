#pragma once
#include "Rect.h"
class PlayerTile
{
	PlayerTile* nextTile = nullptr;
	Rect* r;
public:
	PlayerTile(float x, float y, float dx, float dy);
	PlayerTile(Rect* r);
	void AddTile();
	void MoveTiles();
	void Draw(Brush b);
	bool TileInPosition(float x, float y);
	std::pair<float*, float*> GetPosPtrs();
};

