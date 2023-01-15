#include "Switch.h"

void Switch::Render() {
}

void Switch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void Switch::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{

	}
}

void Switch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SWITCH_BBOX_WIDTH/2;
	t = y - SWITCH_BBOX_HEIGHT/2;
	r = l + SWITCH_BBOX_WIDTH;
	b = t + SWITCH_BBOX_HEIGHT;
}