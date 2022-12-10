#include "Brick.h"
#include "Utils.h"

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;

}
void CBrick::Render()
{
	animation_set->at(BRICK_ANI_IDLE)->Render(x, y);
	RenderBoundingBox();
}
