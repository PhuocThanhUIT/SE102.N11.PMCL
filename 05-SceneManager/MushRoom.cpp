#include "MushRoom.h"
#include "debug.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"
#include "HiddenBrick.h"
CMushRoom::CMushRoom(float x, float y):CGameObject(x,y)
{
	
}

void CMushRoom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
		left = x - MUSHROOM_BBOX_WIDTH/2;
		top = y - MUSHROOM_BBOX_HEIGHT/2;
		right = left + MUSHROOM_BBOX_WIDTH;
		bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushRoom::OnNoCollision(DWORD dt)
{
		x += vx * dt;
		y += vy * dt;
		if(state==MUSHROOM_STATE_MOVE){
			vy = MUSHROOM_GRAVITY;
		}
		
};

void CMushRoom::OnCollisionWith(LPCOLLISIONEVENT e)
{
		if ( e->ny != 0 && e->obj->IsBlocking())
		{
			vy = 0;
		}
		else if (e->nx != 0 && e->obj->IsBlocking())
		{
			vx = -vx;
		}
	
}

void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
		if (start_y - y >= MUSHROOM_BBOX_HEIGHT && state == MUSHROOM_STATE_IDLE){
			this->SetState(MUSHROOM_STATE_MOVE);
		}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CMushRoom::Render()
{
	if (isDeleted) return;
	int aniId = 0;
	if (this->tag == MUSHROOM_GREEN) {
		aniId = 1;
	}
	animation_set->at(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CMushRoom::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vy = MUSHROOM_GROWING_UP_SPEED;
		break;

	case MUSHROOM_STATE_MOVE:
		this->vy = MUSHROOM_GRAVITY;
		vx =-mario->GetMarioDirection()*MUSHROOM_MOVING_SPEED;
		break;
	case MUSHROOM_STATE_DELETE:
		this->Delete();
		break;
	}

}
