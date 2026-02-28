#pragma once
#include "PlayerTile.h"
#include "Defs.h"
#include "TileMap.h"
class Player
{
	float* x;
	float* y;
	PlayerTile* pt;
	Brush* headBrush;
	Brush* tailBrush;
	Brush* midlStrBrush;
	Brush* midlCurBrush;
public:
	int moveDir = 0;
	Player(vec2 RectSize, vec2 startPos);
	float GetPos(int t);
	vec2 GetPos();
	void Move(vec2 moveVector);
	void Draw(bool inTexMode);
	bool PlayerOnTile(int x, int y, TileMap* map);
	void SetBrushes(Brush * headB, Brush * midl1B, Brush * midl2B, Brush* tailB);
	void AddTile();
};

