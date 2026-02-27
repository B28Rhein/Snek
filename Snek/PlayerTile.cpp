#include "PlayerTile.h"

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

void PlayerTile::Draw(Brush b) {
	b.DrawRect(r);
	if (nextTile != nullptr)
		nextTile->Draw(b);
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
