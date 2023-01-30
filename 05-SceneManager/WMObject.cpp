#include "WMObject.h"

CWorldMapObject::CWorldMapObject(int sceneId)
{
	vx = vy = 0;
	SetMove(false, false, false, false);
	this->sceneId = sceneId;
}
void CWorldMapObject::Render()
{
	if (tag == OBJECT_TYPE_HAMMER && vx < 0)
		animation_set->at(1)->Render(x, y);
	else
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CWorldMapObject::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (tag != OBJECT_TYPE_BUSH)
	{
		left = x + 2;
		top = y + 2;
		right = left + OBJECT_BBOX_WIDTH;
		bottom = top + OBJECT_BBOX_WIDTH;
	}
}