#include "Player.h"

Player::Player(vec2 RectSize, vec2 startPos)
{
	pt = new PlayerTile(startPos.x, startPos.y, RectSize.x, RectSize.y);
	auto v = pt->GetPosPtrs();
	x = v.first;
	y = v.second;
}

float Player::GetPos(int t)
{
	if (t == 0) {
		return *x;
	}
	return *y;
}

vec2 Player::GetPos()
{
	return vec2(*x, *y);
}

void Player::Move(vec2 NewPosition) {
	pt->MoveTiles();
	*x = NewPosition.x;
	*y = NewPosition.y;
}

void Player::Draw(bool inTexMode) {
	pt->Draw(headBrush, midlStrBrush, midlCurBrush, tailBrush, true, moveDir, inTexMode);
}

bool Player::PlayerOnTile(int x, int y, TileMap* map) {
	Tile* t = map->GetTile(x, y);
	vec2 coord = t->GetPos();
	return pt->TileInPosition(coord.x, coord.y);
}

void Player::SetBrushes(Brush* headB, Brush* midl1B, Brush* midl2B, Brush* tailB)
{
	headBrush = headB;
	midlStrBrush = midl1B;
	midlCurBrush = midl2B;
	tailBrush = tailB;
}

void Player::AddTile()
{
	pt->AddTile();
}
