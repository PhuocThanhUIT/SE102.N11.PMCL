#include "HiddenBrick.h"

void CHiddenBrick::Render()
{
	//RenderBoundingBox();
}

void CHiddenBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - HIDDEN_BRICK_BBOX_WIDTH/2;
	t = y - HIDDEN_BRICK_BBOX_HEIGHT/2;
	r = l + HIDDEN_BRICK_BBOX_WIDTH;
	b = t + HIDDEN_BRICK_BBOX_HEIGHT;
}