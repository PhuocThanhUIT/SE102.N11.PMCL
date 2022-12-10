#include "Koopa.h"
#include "debug.h"
#include "Brick.h"
#include "Goomba.h"
#include "HiddenBrick.h"
#include "PlayScene.h"

CKoopa::CKoopa(float x, float y)
{
	this->SetState(KOOPA_STATE_NORMAL);
	this->ay = KOOPA_GRAVITY;
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_NORMAL) {
		left = x;
		top = y - KOOPA_BBOX_HEIGHT/4 ;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
	else {
		left = x;
		top = y;
		right = left + KOOPA_SHELL_BBOX_WIDTH;
		bottom = top + KOOPA_SHELL_BBOX_HEIGHT;
	}
		
}

void CKoopa::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CKoopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	if ( e->nx !=0)
	{
		vx = -vx;
	}
	if (dynamic_cast<CHiddenBrick*>(e->obj)) OnCollisionWithHiddenBrick(e);
}

void CKoopa::OnCollisionWithHiddenBrick(LPCOLLISIONEVENT e) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT>* coObjects = currentScene->GetObjects();
	for (int i = 0; i < coObjects->size(); i++) {
		LPGAMEOBJECT obj = coObjects->at(i);
		if (obj==e->obj)
		{
			
			if (vx>0 && state==KOOPA_STATE_NORMAL) {
				LPGAMEOBJECT obj1 = coObjects->at(i+1);
				DebugOut(L"y1:%i,y2:%i", obj1->getY(), obj->getY());
				if (obj1->getY() != obj->getY()) vx=-vx;
			}
			else if(vx<0&&state ==KOOPA_STATE_NORMAL) {
				LPGAMEOBJECT obj1 = coObjects->at(i-1);
				if (obj1->getY() != obj->getY() && obj->getX()>=this->x) vx = -vx;
			}
		}
	}
}
void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	for (int i = 0; i < coObjects->size(); i++) {
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CHiddenBrick*>(obj))
		{
			if (obj->getY() > this->y) {
				obj->SetIsBlocking(1);
			}
			else {
				obj->SetIsBlocking(0);
			}
		}
	}


	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}

int CKoopa::GetAniIdKoopa() {
	int aniId = 0;
	if (state == KOOPA_STATE_NORMAL) {
		if (vx > 0) aniId = KOOPA_WALK_RIGHT_ANI_ID;
		else aniId = KOOPA_WALK_LEFT_ANI_ID;
	}
	if (state == KOOPA_STATE_SHELL) {
		aniId = KOOPA_SHELL_ANI_ID;
	}
	if (state == KOOPA_STATE_SPIN) {
		if (vx > 0) aniId = KOOPA_SPIN_RIGHT_ANI_ID;
		else aniId = KOOPA_SPIN_LEFT_ANI_ID;
	}
	return aniId;
}
void CKoopa::Render()
{
	int aniId = GetAniIdKoopa();
	switch (tag) {
	}
	animation_set->at(aniId)->Render(x, y);
	
	RenderBoundingBox();
}

void CKoopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_STATE_NORMAL:
		vx = -KOOPA_MOVING_SPEED;
		break;
	case KOOPA_STATE_SHELL:
		vx = 0;
	}
}
