#include "MushRoom.h"
#include "debug.h"
#include "Goomba.h"

CMushRoom::CMushRoom(float x, float y):CGameObject(x,y)
{
	
}

void CMushRoom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x;
		top = y;
		right = left + MUSHROOM_BBOX_WIDTH;
		bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushRoom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CMushRoom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CGoomba*>(e->obj)) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (start_y-y>MUSHROOM_BBOX_HEIGHT-1) {
		this->SetState(MUSHROOM_STATE_MOVE);
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CMushRoom::Render()
{
	int aniId = 0;
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CMushRoom::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vy = MUSHROOM_GROWING_UP_SPEED;
		break;

	case MUSHROOM_STATE_MOVE:
		vy = 0;
		break;
	}
}
