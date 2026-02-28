#pragma once
#include "Defs.h"
#include "Window.h"
#include "Player.h"
#include "TileMap.h"
class Snek
{
	Window* window = nullptr;
	Player* player = nullptr;
	TileMap* map = nullptr;
	Brush* letterBrush = nullptr;
	int pointCounter = 0;
	float deltaTime = 0;
	float lastFrame = 0;
	float MoveCounter = 0;
	float moveSpeed = 5;
	bool gameLost = false;
	bool inProgress = true;
#ifdef _DEBUG
	bool debug = true;
#else
	bool debug = false;
#endif
	vec2 GetTileCoordsFromDrawCoords(float X, float Y);
	Tile* GetTileFromDrawCoords(float X, float Y);
	void ShowPoints();
	ivec2 GetLetterOffset(char letter);
public:
	bool isInTextureMode = false;
	int bannedDir = 0;
	char direction = 0;
	Snek();
	Window* GetWindow();
	void CreatePlayer();
	void CreateTilemap(int x, int y);
	void CreateTilemap(int x, int y, float borderLen);
	void CreateTilemap(int x, int y, float borderLen, vec2 offset);
	void SetTilemapBrush(int n, Brush* brush);
	void GenApel();
	void MovePlayer();
	void CalcDeltaTime();
	void SetPlayerBrush(Brush * headBrush, Brush* mid1Brush, Brush * mid2Brush, Brush* tailBrush);
	void SetLetterBrush(Brush* letterBrush);
	void PauseUnpause();
	void Draw();
	void DrawString(std::string s, vec2 pos);
	void DrawMulLines(std::vector<std::string> lines, vec2 pos);
	void DrawLetter(char letter, vec2 pos);
	~Snek();
};

