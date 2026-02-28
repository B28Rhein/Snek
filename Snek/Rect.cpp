#include "Rect.h"

Buffers* Rect::buf = nullptr;

Rect::Rect(float x, float y, float dx, float dy) {
	this->x = x;
	this->y = y;
	this->dx = dx;
	this->dy = dy;
	
}

Rect::Rect(Rect* rect)
{
	this->x =	rect->x;
	this->y =	rect->y;
	this->dx =	rect->dx;
	this->dy =	rect->dy;
}

mat4 Rect::GetModel(float rotation) {
	mat4 model = mat4(1.0f);
	model = glm::translate(model, vec3(x, y, 0));
	model = glm::scale(model, vec3(dx, dy, 1));
	model = glm::rotate(model, glm::radians(rotation), vec3(0, 0, 1.0f));
	return model;
}

std::pair<float*, float*> Rect::GetPosPtrs()
{
	return std::pair<float*, float*>(&x, &y);
}

vec2 Rect::GetPos()
{
	return vec2(x, y);
}

void Rect::SetPos(vec2 npos)
{
	x = npos.x;
	y = npos.y;
}

void Rect::SetPos(float nx, float ny)
{
	x = nx;
	y = ny;
}

uint Rect::GetVao()
{
	return Rect::buf->GetVao();
}
