#include "PlayerTile.h"
#include "Defs.h"

PlayerTile::PlayerTile(float x, float y, float dx, float dy) {
	r = new Rect(x, y, dx, dy);
}

PlayerTile::PlayerTile(Rect* r)
{
	this->r = new Rect(r);
}

void PlayerTile::AddTile() {
	if (nextTile == nullptr) {
		nextTile = new PlayerTile(r);
	}
	else {
		nextTile->AddTile();
	}
}

void PlayerTile::MoveTiles() {
	if (nextTile == nullptr) {
		return;
	}
	if (!(nextTile->r->GetPos() == r->GetPos())) {
		nextTile->MoveTiles();
		nextTile->r->SetPos(r->GetPos());
	}
}

void PlayerTile::Draw(Brush* b1, Brush* b21, Brush* b22, Brush* b3, bool isHead, int dir, bool inTexMode) {
	float rot = GetRotFromDir(dir);
	int nextDir = dir;
	if (nextTile != nullptr) {

		nextDir = GetNextTileDir();
	}
	if (isHead) {
		b1->DrawRect(r, inTexMode, rot);
	}
	else {
		if (nextTile == nullptr) {
			b3->DrawRect(r, inTexMode, rot);
		}
		else if (nextDir == dir || nextDir == ReverseDir(dir)) {
			b21->DrawRect(r, inTexMode, rot);
		}
		else {
			b22->DrawRect(r, inTexMode, getCurRot(rot, GetRotFromDir(ReverseDir(nextDir))));
		}
	}

	if (nextTile != nullptr)
		nextTile->Draw(b1, b21, b22, b3, false, nextDir, inTexMode);
}

float PlayerTile::getCurRot(float rot1, float rot2) {
	if (rot1 == 0 && rot2 == 270 || rot1 == 270 && rot2 == 0) {
		return 0;
	}
	return std::max(rot1, rot2);
}

int PlayerTile::GetNextTileDir() {
	vec2 thisPos = r->GetPos();
	vec2 nextPos = nextTile->r->GetPos();
	vec2 diff = thisPos - nextPos;
	int dir = 0;
	if (diff.x > 0) {
		dir |= 1;
	}
	if (diff.x < 0) {
		dir |= 4;
	}
	if (diff.y > 0) {
		dir |= 8;
	}
	if (diff.y < 0) {
		dir |= 2;
	}
	return dir;
}

int PlayerTile::ReverseDir(int dir) {
	switch (dir) {
	case 1:
		return 4;
		break;
	case 2:
		return 8;
		break;
	case 4:
		return 1;
		break;
	case 8:
		return 2;
		break;
	}
}

float PlayerTile::GetRotFromDir(int dir) {
	switch (dir) {
	case 1:
		return 270;
		break;
	case 2:
		return 180;
		break;
	case 4:
		return 90;
		break;
	case 8:
		return 0;
		break;
	}
}

bool PlayerTile::TileInPosition(float x, float y) {
	if (r->GetPos() == vec2(x, y)) {
		return true;
	}
	if (nextTile == nullptr)
		return false;
	return nextTile->TileInPosition(x, y);
}

std::pair<float*, float*> PlayerTile::GetPosPtrs()
{
	return r->GetPosPtrs();
}
