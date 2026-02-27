#pragma once
#include "PlayerTile.h"
#include "Defs.h"
#include "TileMap.h"
class Player
{
	float* x;
	float* y;
	PlayerTile* pt;
	Brush* brush;
public:
	Player(vec2 RectSize, vec2 startPos);
	float GetPos(int t);
	vec2 GetPos();
	void Move(vec2 moveVector);
	void Draw();
	bool PlayerOnTile(int x, int y, TileMap* map);
	void SetBrush(Brush*);
	void AddTile();
};

