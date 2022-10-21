#include "HiddenBrick.h"

void CHiddenBrick::Render()
{
	//RenderBoundingBox();
}

void CHiddenBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + HIDDEN_BRICK_BBOX_WIDTH;
	b = y + HIDDEN_BRICK_BBOX_HEIGHT;
}