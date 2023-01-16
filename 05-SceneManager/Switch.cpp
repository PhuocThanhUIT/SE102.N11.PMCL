#include "Switch.h"

void Switch::Render() {
	if (state == SWITCH_STATE_PRESSED)
		animation_set->at(SWITCH_ANI_PRESSED)->Render(x, y);
	else
	animation_set->at(SWITCH_ANI_IDLE)->Render(x, y);
	//RenderBoundingBox();
}

void Switch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isDeleted)
		return;
	CGameObject::Update(dt);
	if (state == SWITCH_STATE_UP)
	{
		y += vy * dt;
		if (start_y - y >= SWITCH_BBOX_HEIGHT)
		{
			vy = 0;
			y = start_y - SWITCH_BBOX_HEIGHT - 0.1f;
			SetState(SWITCH_STATE_IDLE);
		}
	}
}

void Switch::SetState(int state) {
	CGameObject::SetState(state);
	switch (state)
	{
	case SWITCH_STATE_UP:
		vy = -0.05f;
		break;
	case SWITCH_STATE_PRESSED:
		ChangeBreakBrickToCoin();
		break;
	}
}

void Switch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SWITCH_BBOX_WIDTH/2;
	t = y - SWITCH_BBOX_HEIGHT/2;
	r = l + SWITCH_BBOX_WIDTH;
	b = t + SWITCH_BBOX_HEIGHT;
}

void Switch::ChangeBreakBrickToCoin() {

}