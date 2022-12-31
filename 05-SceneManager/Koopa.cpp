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
	this->ax = 0;
}

void CKoopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_STATE_NORMAL) {
		left = x- KOOPA_BBOX_WIDTH/2;
		top = y - KOOPA_BBOX_HEIGHT/2 ;
		right = left + KOOPA_BBOX_WIDTH;
		bottom = top + KOOPA_BBOX_HEIGHT;
	}
	else {
		left = x -KOOPA_SHELL_BBOX_WIDTH/2;
		top = y - KOOPA_SHELL_BBOX_HEIGHT/2;
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
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	if ( e->nx !=0 && e->obj->IsBlocking())
	{
		if (!dynamic_cast<CHiddenBrick*>(e->obj)) {
			x = e->obj->getX();
			vx = -vx;
		}
		else {
			//CHiddenBrick* hiddenbrick = dynamic_cast<CHiddenBrick*>(e->obj);
			//hiddenbrick->SetIsBlocking(0);
		}
	}
	if (dynamic_cast<CHiddenBrick*>(e->obj)) OnCollisionWithHiddenBrick(e);
	if (dynamic_cast<CGoomba*>(e->obj)) OnCollisionWithGoomba(e);
}

void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e) {
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (state == KOOPA_STATE_SPIN)
	{
		goomba->SetState(GOOMBA_STATE_DIE_BY_KOOPA);
	}

}
void CKoopa::OnCollisionWithHiddenBrick(LPCOLLISIONEVENT e) {
	if ( state == KOOPA_STATE_NORMAL)
	{
		if (vx > 0 && x >= e->obj->x+4)
			if (CalTurnableRight(e->obj))
			{
				vx = -vx;
			}
		if (vx < 0)
			if (CalTurnableLeft(e->obj))
			{		
				vx = -vx;
			}
	}
	
}
bool CKoopa::CalTurnableRight(LPGAMEOBJECT object)
{
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> coObjects = currentScene->GetObjects();
	for (UINT i = 0; i < coObjects.size(); i++) {
		if (coObjects.at(i) == object)
		{
			if (coObjects.at(i + 1)->y != coObjects.at(i)->y) {
				 return true; 
			}
			else return false;
		}
	}
	return false;
}
bool CKoopa::CalTurnableLeft(LPGAMEOBJECT object)
{
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> coObjects = currentScene->GetObjects();
	for (UINT i = 0; i < coObjects.size(); i++) {
		if (coObjects.at(i) == object)
		{
			if (coObjects.at(i-1)->y != coObjects.at(i)->y&&x<= coObjects.at(i)->x-4) {
				return true;
			}
			else return false;
		}
	}
	return false;
}
void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	DebugOutTitle(L"x:%f,y:%f", x, y);
	if (GetTickCount64() - shell_start >= KOOPA_SHELL_TIME && shell_start != 0 && state != KOOPA_STATE_SPIN) {
		shell_start = 0;
		StartReviving();
	}

	if (GetTickCount64() - reviving_start >= KOOPA_REVIVE_TIME && reviving_start != 0 && state != KOOPA_STATE_SPIN && shell_start == 0)
	{
		reviving_start = 0;
		y -= (KOOPA_BBOX_HEIGHT - KOOPA_SHELL_BBOX_HEIGHT) + 1.0f;
		SetState(KOOPA_STATE_NORMAL);
		if (isBeingHeld) {
			this->isBeingHeld = false;
			mario->isHolding = false;
			this->SetVx(mario->nx * KOOPA_MOVING_SPEED);
		}
	}
	if (!isBeingHeld) {
		vy += ay * dt;
	}
	HandleBeingHeld(mario);

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CKoopa::HandleBeingHeld(LPGAMEOBJECT object) {
	CMario* mario = dynamic_cast<CMario*>(object);
	if (mario->isHolding && this->isBeingHeld) {
		if(mario->nx>0) x = mario->x + MARIO_SMALL_BBOX_WIDTH*mario->nx -3.0f;
		else  x = mario->x + MARIO_SMALL_BBOX_WIDTH*mario->nx;
		y = mario->y - 2.0f;
	}
	else if(!mario->isHolding && this->isBeingHeld) {
		this->isBeingHeld = false;
		this->SetState(KOOPA_STATE_SPIN);
		this->SetVx(mario->nx * KOOPA_SPIN_SPEED);
	}
}
int CKoopa::GetAniIdKoopa() {
	int aniId = 0;
	if (state == KOOPA_STATE_NORMAL) {
		if (vx > 0) aniId = KOOPA_WALK_RIGHT_ANI_ID;
		else aniId = KOOPA_WALK_LEFT_ANI_ID;
	}
	if (state == KOOPA_STATE_SHELL) {
		if (reviving_start != 0) {
			aniId = KOOPA_SHAKE_ANI_ID;
		}else
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
	if (isDeleted) return;
	int aniId = GetAniIdKoopa();
	switch (tag) {
	}
	animation_set->at(aniId)->Render(x, y);
	
	//RenderBoundingBox();
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
		StartShell();
	}
}
