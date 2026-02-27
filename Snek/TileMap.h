#pragma once
#include "Tile.h"
class TileMap
{
	Tile** map;
	int x;
	int y;
	float tileSizeX = 0;
	float tileSizeY = 0;
	vec2 offset;
	Brush** StateBrushes;
	Brush* OutBrush;
public:
	TileMap(int x, int y, float WinX, float WinY);
	TileMap(int x, int y, float len);
	TileMap(int x, int y, float len, vec2 offset);
	void SetBrush(int BrushNo, Brush* brush);
	void Draw();
	vec2 GetTileSize();
	vec4 GetBoundries();
	Tile* GetTile(int x, int y);
	vec2 GetSize();
	int GetSize(int n);
	vec2 GetMiddleTile();
	vec2 GetOffset();
};

