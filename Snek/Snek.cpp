#include "Snek.h"


void Snek::MovePlayer() {
	MoveCounter += deltaTime;
	vec2 TileSize = map->GetTileSize();
	if (MoveCounter >= (1 / moveSpeed) && inProgress) {

		char direction = this->direction;
		MoveCounter = 0;
		vec2 moveVector = vec2(0.0f);
		if (direction % 2 == 1) {
			moveVector += vec2(1.0, 0.0) * TileSize.x;
			bannedDir = 4;
		}
		direction >>= 1;
		if (direction % 2 == 1) {
			moveVector += -vec2(0.0, 1.0) * TileSize.y;
			bannedDir = 8;
		}
		direction >>= 1;
		if (direction % 2 == 1) {
			moveVector += -vec2(1.0, 0.0) * TileSize.x;
			bannedDir = 1;
		}
		direction >>= 1;
		if (direction % 2 == 1) {
			moveVector += vec2(0.0, 1.0) * TileSize.y;
			bannedDir = 2;
		}
		direction >>= 1;
		float playerNewX = player->GetPos(0) + moveVector.x;
		float playerNewY = player->GetPos(1) + moveVector.y;
		vec4 boundries = map->GetBoundries();
		if (playerNewX < boundries.x-1) {
			playerNewX = boundries.z;
		}
		if (playerNewX > boundries.z+1) {
			playerNewX = boundries.x;
		}
		if (playerNewY > boundries.y+1) {
			playerNewY = boundries.w;
		}
		if (playerNewY < boundries.w-1) {
			playerNewY = boundries.y;
		}
		Tile* t = GetTileFromDrawCoords(playerNewX, playerNewY);
		vec2 tileCoords = GetTileCoordsFromDrawCoords(playerNewX, playerNewY);
		if (player->PlayerOnTile((int)tileCoords.x, (int)tileCoords.y, map) && (player->GetPos(0) != playerNewX || player->GetPos(1) != playerNewY)) {
			gameLost = true;
			inProgress = false;
		}
		playerNewX = t->GetPos().x;
		playerNewY = t->GetPos().y;
		if (t->GetState() == 1) {
			pointCounter += 1;
			t->SetState(0);
			GenApel();
			player->AddTile();
			moveSpeed += 0.5;
		}
		if (t->GetState() != 2) {
			player->Move(vec2(playerNewX, playerNewY));
		}

	}
	ShowPoints();

}

void Snek::CalcDeltaTime() {
	float currentFrame = window->GetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Snek::SetPlayerBrush(Brush* brush)
{
	player->SetBrush(brush);
}

void Snek::Draw()
{
	map->Draw();
	player->Draw();
}

void Snek::GenApel() {
	std::random_device rand;
	std::uniform_int_distribution<int> distX(0, map->GetSize(0) - 1);
	std::uniform_int_distribution<int> distY(0, map->GetSize(1) - 1);
	int y = 0;
	int x = 0;
	do {
		x = distX(rand);
		y = distY(rand);
	} while (player->PlayerOnTile(x, y, map));
	Tile* t = map->GetTile(x, y);
	t->SetState(1);
}

vec2 Snek::GetTileCoordsFromDrawCoords(float X, float Y) {
	vec2 offset = map->GetOffset();
	X -= offset.x;
	Y -= offset.y;
	float smallerDim = map->GetTileSize().x * map->GetSize(0);
	int W = round((X + smallerDim / 2 - map->GetTileSize().x / 2) / map->GetTileSize().x);
	//	X = -smallerDim/2 + map->tileSizeX/2 + map->tileSizeX * W
	// X + smallerDim/2 - map->tileSizeX/2 = map->tileSizeX * W
	// W = (X + smallerDim/2 - map->tileSizeX/2)/map->tileSizeX
	//	
	int U = round(-(Y - smallerDim / 2 + map->GetTileSize().y / 2) / map->GetTileSize().y);
	return vec2(W, U);
}

Tile* Snek::GetTileFromDrawCoords(float X, float Y)
{
	vec2 coords = GetTileCoordsFromDrawCoords(X, Y);
	Tile* t = map->GetTile(coords.x, coords.y);
	return t;
}

void Snek::ShowPoints() {
	system("cls");
	if (inProgress && debug)
		std::cout
		<< pointCounter << "\n"
		<< MoveCounter << "\n"
		<< deltaTime << "\n"
		<< 1 / moveSpeed << "\n"
		<< glm::to_string(map->GetBoundries())
		<< "\n"
		<< glm::to_string(player->GetPos())
		<< "\n"
		<< (int)direction
		<< "\n"
		<< bannedDir;
	else if (inProgress) {
		std::cout << pointCounter;
	}
	else if (gameLost) {
		std::cout << "YOU'VE LOST";
	}
}

Window* Snek::GetWindow()
{
	return window;
}

void Snek::CreatePlayer()
{
	player = new Player(map->GetTileSize(), map->GetMiddleTile());
}

void Snek::CreateTilemap(int x, int y)
{
	auto WinSize = window->GetSize();
	map = new TileMap(x, y, WinSize.x, WinSize.y);
}
void Snek::CreateTilemap(int x, int y, float borderLen)
{
	map = new TileMap(x, y, borderLen);
}
void Snek::CreateTilemap(int x, int y, float borderLen, vec2 offset)
{
	map = new TileMap(x, y, borderLen, offset);
}

void Snek::SetTilemapBrush(int n, Brush* brush)
{
	map->SetBrush(n, brush);
}

Snek::Snek()
{
	window = new Window();
}

Snek::~Snek()
{
	delete window;
	delete player;
	delete map;
}
