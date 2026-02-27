#include "TileMap.h"

TileMap::TileMap(int x, int y, float WinX, float WinY) {
	map = new Tile * [x * y];
	this->x = x;
	this->y = y;
	float smallerDim = WinY;
	if (WinX < smallerDim) smallerDim = WinX;
	smallerDim *= 0.95;
	float dx = smallerDim / x;
	float dy = smallerDim / y;
	for (int i = 0; i < x * y; i++) {
		map[i] = new Tile(-smallerDim / 2 + dx / 2 + dx * (i % x),
			smallerDim / 2 - dy / 2 - dy * (i / y),
			dx,
			dy,
			0);
	}
	tileSizeX = dx;
	tileSizeY = dy;
	StateBrushes = new Brush*[10];
	offset = vec2(0);
}
TileMap::TileMap(int x, int y, float len) {
	map = new Tile*[x * y];
	this->x = x;
	this->y = y;
	float smallerDim = len;
	smallerDim *= 0.95;
	float dx = smallerDim / x;
	float dy = smallerDim / y;
	for (int i = 0; i < x * y; i++) {
		map[i] = new Tile(-smallerDim / 2 + dx / 2 + dx * (i % x),
			smallerDim / 2 - dy / 2 - dy * (i / y),
			dx,
			dy,
			0);
	}
	tileSizeX = dx;
	tileSizeY = dy;
	StateBrushes = new Brush*[10];
	offset = vec2(0);
}
TileMap::TileMap(int x, int y, float len, vec2 offset) {
	map = new Tile * [x * y];
	this->x = x;
	this->y = y;
	float smallerDim = len;
	smallerDim *= 0.95;
	float dx = smallerDim / x;
	float dy = smallerDim / y;
	for (int i = 0; i < x * y; i++) {
		float posx = -smallerDim / 2 + dx / 2 + dx * (i % x);
		float posy =  smallerDim / 2 - dy / 2 - dy * (i / y);
		posx += offset.x;
		posy += offset.y;
		map[i] = new Tile(
			posx,
			posy,
			dx,
			dy,
			0);
	}
	tileSizeX = dx;
	tileSizeY = dy;
	StateBrushes = new Brush*[10];
	this->offset = offset;
}

void TileMap::SetBrush(int BrushNo, Brush* brush)
{
	if (BrushNo == -1) {
		OutBrush = brush;
		return;
	}
	StateBrushes[BrushNo] = brush;
}

void TileMap::Draw() {
	for (int i = 0; i < x * y; i++) {
		
		map[i]->Draw(*OutBrush, *StateBrushes[map[i]->GetState()]);

	}
}

vec2 TileMap::GetTileSize()
{
	return vec2(tileSizeX, tileSizeY);
}

vec4 TileMap::GetBoundries()
{
	vec2 upLeft = map[0]->GetPos();
	vec2 downRight = map[x * y - 1]->GetPos();
	vec4 boundries = vec4(upLeft.x, upLeft.y, downRight.x, downRight.y);
	return boundries;
}

Tile* TileMap::GetTile(int x, int y)
{
	return map[y * this->y + x];
}

vec2 TileMap::GetSize()
{
	return vec2(x, y);
}

int TileMap::GetSize(int n)
{
	if (n == 0) {
		return x;
	}
	return y;
}

vec2 TileMap::GetMiddleTile()
{
	vec2 tPos = vec2(floor(x / 2), floor(y / 2));
	Tile* t = GetTile(tPos.x, tPos.y);
	return vec2(t->GetPos());
}

vec2 TileMap::GetOffset()
{
	return offset;
}
