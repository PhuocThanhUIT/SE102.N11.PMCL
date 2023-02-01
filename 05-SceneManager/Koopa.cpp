#include "Koopa.h"
#include "debug.h"
#include "Brick.h"
#include "Goomba.h"
#include "HiddenBrick.h"
#include "PlayScene.h"
#include "QuestionBrick.h"
#include "BreakBrick.h"
#include "PiranhaPlantFire.h"


CKoopa::CKoopa(int tag){
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	mario = currentScene->GetPlayer();
	this->tag = tag;
	if (tag == KOOPA_GREEN_PARA) {
		this->SetState(KOOPA_STATE_JUMP);
	}
	else {
		this->SetState(KOOPA_STATE_NORMAL);
	}
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
		if (state == KOOPA_STATE_JUMP) vy = -KOOPA_JUMP_SPEED;
		else
		vy = 0;
	}
	if ( e->nx !=0 && e->obj->IsBlocking())
	{
		if (!dynamic_cast<CHiddenBrick*>(e->obj)) {
			vx = -vx;
		}
		else {
			CHiddenBrick* hiddenbrick = dynamic_cast<CHiddenBrick*>(e->obj);
			hiddenbrick->SetIsBlocking(0);
		}
	}
	if (dynamic_cast<CHiddenBrick*>(e->obj)) OnCollisionWithHiddenBrick(e);
	if (dynamic_cast<CGoomba*>(e->obj)) OnCollisionWithGoomba(e);
	if (dynamic_cast<CQuestionBrick*>(e->obj)) OnCollisionWithQuestionBrick(e);
	if (dynamic_cast<BreakableBrick*>(e->obj)) OnCollisionWithBreakableBrick(e);
	if (dynamic_cast<CPiranhaPlantFire*>(e->obj)) OnCollisionWithPiranhaPlamtFire(e);
}
void CKoopa::OnCollisionWithPiranhaPlamtFire(LPCOLLISIONEVENT e) {
	CPiranhaPlantFire* plant = dynamic_cast<CPiranhaPlantFire*>(e->obj);
	if (state == KOOPA_STATE_SPIN) {
		plant->SetState(PIRANHAPLANT_STATE_DEATH);
	}
}
void CKoopa::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e) {
	CQuestionBrick* qsbrick = dynamic_cast<CQuestionBrick*>(e->obj);
	if (e->nx!=0&&qsbrick->GetState() != QUESTION_BRICK_STATE_AFTER_MARIO_PUSH_UP && state==KOOPA_STATE_SPIN) {
		qsbrick->ShowItem(qsbrick->tag);
		qsbrick->SetState(QUESTION_BRICK_STATE_AFTER_MARIO_PUSH_UP);
	}
}
void CKoopa::OnCollisionWithGoomba(LPCOLLISIONEVENT e) {
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (state == KOOPA_STATE_SPIN && goomba->GetState() != GOOMBA_STATE_DIE_BY_KOOPA)
	{
		mario->AddScore(goomba->x, goomba->y, 100);
		goomba->SetState(GOOMBA_STATE_DIE_BY_KOOPA);
	}

}
void CKoopa::OnCollisionWithHiddenBrick(LPCOLLISIONEVENT e) {
	if ( state == KOOPA_STATE_NORMAL && tag == KOOPA_RED)
	{
		if (vx > 0 && x >= e->obj->x+4)
			if (CalTurnable(e->obj))
			{
				vx = -vx;
			}
		if (vx < 0 && x <= e->obj->x - 4)
			if (CalTurnable(e->obj))
			{		
				vx = -vx;
			}
	}
}
void CKoopa::OnCollisionWithBreakableBrick(LPCOLLISIONEVENT e) {
	if (state == KOOPA_STATE_SPIN) {
		if (e->nx != 0) {
			BreakableBrick* brk = dynamic_cast<BreakableBrick*>(e->obj);
			brk->Break();
		}
	}
	if (state == KOOPA_STATE_NORMAL && tag == KOOPA_RED)
	{
		if (vx > 0 && x >= e->obj->x + 4)
			if (CalTurnable(e->obj))
			{
				vx = -vx;
			}
		if (vx < 0 && x <= e->obj->x - 4)
			if (CalTurnable(e->obj))
			{
				vx = -vx;
			}
	}
}
bool CKoopa::CalTurnable(LPGAMEOBJECT object)
{
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	vector<LPGAMEOBJECT> coObjects = currentScene->GetObjects();
	for (UINT i = 0; i < coObjects.size(); i++)
		if (dynamic_cast<CHiddenBrick*>(coObjects[i]) || dynamic_cast<CQuestionBrick*>(coObjects[i])||dynamic_cast<BreakableBrick*>(coObjects[i]))
			if (abs(coObjects[i]->y == object->y))
			{
				if (vx > 0)
					if (coObjects[i]->x > object->x && coObjects[i]->x - 16 < object->x + 16)
					{
						return false;
					}
				if (vx < 0)
					if (coObjects[i]->x + 16 > object->x - 16 && coObjects[i]->x < object->x)
					{
						return false;
					}
			}
	return true;
}

void CKoopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (isActive) {
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

		// for tail collision
		CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = currentScene->GetPlayer();
		float mLeft, mTop, mRight, mBottom;
		float oLeft, oTop, oRight, oBottom;
		if (mario != NULL && state != KOOPA_STATE_DIE) {
			if (mario->isTailAttack && mario->level == MARIO_LEVEL_TAIL) {
				mario->tail->GetBoundingBox(mLeft, mTop, mRight, mBottom);
				GetBoundingBox(oLeft, oTop, oRight, oBottom);
				if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom))
				{
					nx = mario->nx;
					SetState(KOOPA_STATE_DIE);
				}
			}
		}

		HandleBeingHeld(mario);

		CGameObject::Update(dt, coObjects);
		CCollision::GetInstance()->Process(this, dt, coObjects);
	}
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
	if (state == KOOPA_STATE_JUMP) {
		if (vx > 0) aniId = KOOPA_PARA_RIGHT_ANI_ID;
		else aniId = KOOPA_PARA_LEFT_ANI_ID;
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
	if (state == KOOPA_STATE_DIE) {
		aniId = KOOPA_DIE_ANI_ID;
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
	case KOOPA_STATE_JUMP:
		vx = -KOOPA_MOVING_SPEED;
		ay = 0.0005f;
		break;
	case KOOPA_STATE_NORMAL:
		vx = -KOOPA_MOVING_SPEED;
		ay = 0.002f;
		break;
	case KOOPA_STATE_SPIN:
		ay = 0.0005f;
		break;
	case KOOPA_STATE_SHELL:
		vx = 0;
		StartShell();
		break;
	case KOOPA_STATE_DIE:
		vy = -KOOPA_DIE_DEFLECT_SPEED;
		ay = KOOPA_GRAVITY;
	}
}
