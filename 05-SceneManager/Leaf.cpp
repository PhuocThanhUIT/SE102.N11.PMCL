#include "Leaf.h"
#include "debug.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"

CLeaf::CLeaf(float x, float y) :CGameObject(x, y) {

}
void CLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - LEAF_BBOX_WIDTH / 2;
	top = y - LEAF_BBOX_HEIGHT / 2;
	right = left + LEAF_BBOX_WIDTH;
	bottom = top + LEAF_BBOX_HEIGHT;
}




void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	x += vx * dt;
	y += vy * dt;
	if (state == LEAF_STATE_FALLING) {
		if (GetTickCount64() - start_timing >= LEAF_FALLING_TIME) {
			vx = -vx;
			StartTiming();
		}
	}
	if (state == LEAF_STATE_UP) {
		if (start_y - y >= LEAF_UP_HEIGHT) {
			SetState(LEAF_STATE_FALLING);
		}
	}
	CGameObject::Update(dt, coObjects);
}


void CLeaf::Render()
{
	if (isDeleted) return;
	int aniId = 0;
	if (state == LEAF_STATE_FALLING)
	{
		if (vx >= 0)
			aniId = LEAF_ANI_RIGHT;
		else
			aniId = LEAF_ANI_LEFT;
	}
	animation_set->at(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_IDLE:
		vx = vy = 0;
		break;
	case LEAF_STATE_UP:
		vy = -LEAF_SPEED_UP;
		vx = 0;
		break;
	case LEAF_STATE_FALLING:
		vy = LEAF_GRAVITY;
		vx = LEAF_SPEED;
		StartTiming();
		break;
	}

}
