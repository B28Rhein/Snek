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
	void Draw(Brush* b1, Brush* b21, Brush * b22, Brush* b3, bool isHead, int dir, bool inTexMode);
	float getCurRot(float rot1, float rot2);
	int GetNextTileDir();
	int ReverseDir(int dir);
	float GetRotFromDir(int dir);
	bool TileInPosition(float x, float y);
	std::pair<float*, float*> GetPosPtrs();
};

