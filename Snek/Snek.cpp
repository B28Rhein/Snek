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
			return;
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
			player->moveDir = this->direction;
			player->Move(vec2(playerNewX, playerNewY));
		}

	}
	//ShowPoints();

}

void Snek::CalcDeltaTime() {
	float currentFrame = window->GetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void Snek::SetPlayerBrush(Brush* headBrush, Brush* mid1Brush, Brush* mid2Brush, Brush* tailBrush)
{
	player->SetBrushes(headBrush, mid1Brush, mid2Brush, tailBrush);
}

void Snek::SetLetterBrush(Brush* letterBrush)
{
	this->letterBrush = letterBrush;
}

void Snek::PauseUnpause()
{
	if (inProgress) {
		inProgress = false;
		return;
	}
	if (!inProgress) {
		inProgress = true;
	}
}

void Snek::Draw()
{
	map->Draw(isInTextureMode);
	player->Draw(isInTextureMode);
	std::vector<std::string> linesToDraw = std::vector<std::string>();
	std::string s = "You've got: ";
	std::string s2 = std::to_string(pointCounter);
	std::string s3 = " points";
	std::string sc = s + s2 + s3;
	
	std::string s4 = "FPS: " + std::to_string(1 / deltaTime);
	std::string lostS = "YOU'VE LOST";
	vec2 textPos = vec2(-window->GetSize().x / 2 + 50, window->GetSize().y / 2 - 50);
	linesToDraw.push_back(sc);
	if (debug)
		linesToDraw.push_back(s4);
	if (gameLost)
		linesToDraw.push_back(lostS);
	DrawMulLines(linesToDraw, textPos);
}

void Snek::DrawString(std::string s, vec2 pos) {
	for (int i = 0; i < s.length(); i++) {
		DrawLetter(s[i], vec2(pos.x + 12 * i, pos.y));

	}
}
void Snek::DrawMulLines(std::vector<std::string> lines, vec2 pos) {
	for (int i = 0; i < lines.size(); i++) {
		DrawString(lines[i], pos + vec2(0, -25 * i));
	}
}

void Snek::DrawLetter(char letter, vec2 pos)
{
	ivec2 offset = GetLetterOffset(letter);
	Rect* r = new Rect(pos.x, pos.y, 12, 20);
	letterBrush->DrawLetter(r, offset);
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
		<< 1/deltaTime << "\n"
		<< MoveCounter << "\n"
		<< deltaTime << "\n"
		<< 1 / moveSpeed << "\n"
		<< glm::to_string(map->GetBoundries())
		<< "\n"
		<< glm::to_string(player->GetPos())
		<< "\n"
		<< (int)direction
		<< "\n"
		<< bannedDir
		<<"\n"
		<< player->moveDir;
	else if (inProgress) {
		std::cout << pointCounter;
	}
	else if (gameLost) {
		std::cout << "YOU'VE LOST";
	}
}

ivec2 Snek::GetLetterOffset(char letter)
{
	if (letter >= 'A' && letter <= 'Z') {
		ivec2 A = ivec2(0, 6);
		int diff = letter - 'A';
		return ivec2(A + ivec2(diff % 13, -diff / 13));
	}
	if (letter >= 'a' && letter <= 'z') {
		ivec2 A = ivec2(0, 4);
		int diff = letter - 'a';
		return ivec2(A + ivec2(diff % 13, -diff / 13));
	}
	if (letter >= '0' && letter <= '9') {
		ivec2 A = ivec2(0, 2);
		int diff = letter - '0';
		return ivec2(A + ivec2(diff % 13, -diff / 13));
	}
	switch (letter) {
	case '+':
		return ivec2(10, 2);
	case '-':
		return ivec2(11, 2);
	case '=':
		return ivec2(12, 2);
	case '(':
		return ivec2(0, 1);
	case ')':
		return ivec2(1, 1);
	case '[':
		return ivec2(2, 1);
	case ']':
		return ivec2(3, 1);
	case '{':
		return ivec2(4, 1);
	case '}':
		return ivec2(5, 1);
	case '<':
		return ivec2(6, 1);
	case '>':
		return ivec2(7, 1);
	case '/':
		return ivec2(8, 1);
	case '*':
		return ivec2(9, 1);
	case ':':
		return ivec2(10, 1);
	case '#':
		return ivec2(11, 1);
	case '%':
		return ivec2(12, 1);
	case '!':
		return ivec2(0, 0);
	case '?':
		return ivec2(1, 0);
	case '.':
		return ivec2(2, 0);
	case ',':
		return ivec2(3, 0);
	case '\'':
		return ivec2(4, 0);
	case '\"':
		return ivec2(5, 0);
	case '@':
		return ivec2(6, 0);
	case '&':
		return ivec2(7, 0);
	case '$':
		return ivec2(8, 0);
	case ' ':
		return ivec2(9, 0);
	default:
		break;
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
