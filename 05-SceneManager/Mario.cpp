#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "HiddenBrick.h"

#include "Collision.h"
#include "QuestionBrick.h"
#include "MushRoom.h"
#include "Koopa.h"
#include "PiranhaPlantFire.h"
#include "FireBullet.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	isOnPlatform = false;

	

	CCollision::GetInstance()->Process(this, dt, coObjects);
}
int CMario::GetMarioDirection() {
	return this->ax > 0 ? 1 : -1;
}
void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!dynamic_cast<CHiddenBrick*>(e->obj)) {
		if (e->ny != 0 && e->obj->IsBlocking())
		{
			vy = 0;
			if (e->ny < 0) isOnPlatform = true;
		}
		else
			if (e->nx != 0 && e->obj->IsBlocking())
			{
				vx = 0;
			}
	}
	
	if (dynamic_cast<CGoomba*>(e->obj)) {
		DebugOut(L"MARIO COLLISON WITH GOOMBA\n");
		OnCollisionWithGoomba(e);
	}
	else if (dynamic_cast<CQuestionBrick*>(e->obj)) {
		DebugOut(L"MARIO COLLISON WITH QUESTION BRICK\n");
		OnCollisionWithQuestionBrick(e);
	}
	else if (dynamic_cast<CCoin*>(e->obj)) {
		DebugOut(L"MARIO COLLISON WITH COIN\n");
		OnCollisionWithCoin(e);
	}
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CMushRoom*>(e->obj))
		OnCollisionWithMushRoom(e);
	else if (dynamic_cast<CKoopa*>(e->obj)) {
		DebugOut(L"MARIO COLLISON WITH KOOPA\n");
		OnCollisionWithKoopa(e);
	}
	else if (dynamic_cast<CPiranhaPlantFire*>(e->obj))
		OnCollisionWithPiranhaFire(e);
	else if (dynamic_cast<FireBullet*>(e->obj))
		OnCollisionWithFireBullet(e);
}
void CMario::OnCollisionWithFireBullet(LPCOLLISIONEVENT e) {
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			level -= 1;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}
void CMario::OnCollisionWithPiranhaFire(LPCOLLISIONEVENT e) {
	if (untouchable == 0)
	{
		if (level > MARIO_LEVEL_SMALL)
		{
			level -= 1;
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}
void CMario::OnCollisionWithMushRoom(LPCOLLISIONEVENT e) {
	CMushRoom* mushroom = dynamic_cast<CMushRoom*>(e->obj);
	mushroom->SetState(MUSHROOM_STATE_DELETE);
	this->SetLevel(MARIO_LEVEL_BIG);
}
void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
			koopa->SetState(KOOPA_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else 
	{  
		if (koopa->GetState() == KOOPA_STATE_SHELL) {
			if (isReadyToHold) {
				this->isHolding = true;
				koopa->SetIsBeingHeld(true);
			}
			else {
				koopa->SetState(KOOPA_STATE_SPIN);
				if (e->nx < 0) koopa->SetVx(KOOPA_SPIN_SPEED);
				else koopa->SetVx(-KOOPA_SPIN_SPEED);
			}	
		}
		else {
			if (untouchable == 0)
			{
					if (level > MARIO_LEVEL_SMALL)
					{
						level -=1;
						StartUntouchable();
					}
					else
					{
						DebugOut(L">>> Mario DIE >>> \n");
						SetState(MARIO_STATE_DIE);
					}
			}
		}

	}
}
void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_BY_KOOPA)
		{
			if (goomba->GetTag() == NORMAL_GOOMBA || goomba->GetTag() == RED_GOOMBA) {
				goomba->SetState(GOOMBA_STATE_DIE);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else {
				goomba->SetTag(RED_GOOMBA);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE && goomba->GetState() != GOOMBA_STATE_DIE_BY_KOOPA)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}
void CMario::OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e) {
	CQuestionBrick* questionbrick = dynamic_cast<CQuestionBrick*>(e->obj);
	if (e->ny > 0)
	{
		if (questionbrick->GetState() != QUESTION_BRICK_STATE_AFTER_MARIO_PUSH_UP) {
			questionbrick->SetState(QUESTION_BRICK_STATE_MARIO_PUSH_UP);
		}
	}
}
void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (isHolding) {
				if (nx >= 0)
					aniId = MARIO_ANI_SMALL_HOLD_JUMPINGUP_RIGHT;
				else
					aniId = MARIO_ANI_SMALL_HOLD_JUMPINGUP_LEFT;
			}
			else {
				if (nx >= 0)
					aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
				else
					aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
			}	
		}
		else
		{
			if (isHolding) {
				if (nx >= 0)
					aniId = MARIO_ANI_SMALL_HOLD_JUMPINGUP_RIGHT;
				else
					aniId = MARIO_ANI_SMALL_HOLD_JUMPINGUP_LEFT;
			}
			else {
				if (nx >= 0)
					aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
				else
					aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
			}
			
		}
	}
	else
		if (isSitting)
		{
			if (isHolding) {
				if (nx > 0)
					aniId = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT;
				else
					aniId = MARIO_ANI_SMALL_HOLD_IDLE_LEFT;
			}
			else {
				if (nx > 0)
					aniId = MARIO_ANI_SMALL_IDLE_RIGHT;
				else
					aniId = MARIO_ANI_SMALL_IDLE_LEFT;
			}
		}
		else
			if (vx == 0)
			{
				if (isHolding) {
					if (nx > 0)
						aniId = MARIO_ANI_SMALL_HOLD_IDLE_RIGHT;
					else
						aniId = MARIO_ANI_SMALL_HOLD_IDLE_LEFT;
				}
				else {
					if (nx > 0)
						aniId = MARIO_ANI_SMALL_IDLE_RIGHT;
					else
						aniId = MARIO_ANI_SMALL_IDLE_LEFT;
				}
			}
			else if (vx > 0)
			{
				if (isHolding) {
					if (ax < 0)
						aniId = MARIO_ANI_SMALL_HOLD_BRAKING_RIGHT;
					else if (ax == MARIO_ACCEL_RUN_X)
						aniId = MARIO_ANI_SMALL_HOLD_RUNNING_RIGHT;
					else if (ax == MARIO_ACCEL_WALK_X)
						aniId = MARIO_ANI_SMALL_HOLD_WALKING_RIGHT;
				}
				else {
					if (ax < 0)
						aniId = MARIO_ANI_SMALL_BRAKING_RIGHT;
					else if (ax == MARIO_ACCEL_RUN_X)
						aniId = MARIO_ANI_SMALL_RUNNING_RIGHT;
					else if (ax == MARIO_ACCEL_WALK_X)
						aniId = MARIO_ANI_SMALL_WALKING_RIGHT;
				}
				
			}
			else // vx < 0
			{
				if (isHolding) {
					if (ax > 0)
						aniId = MARIO_ANI_SMALL_HOLD_BRAKING_LEFT;
					else if (ax == -MARIO_ACCEL_RUN_X)
						aniId = MARIO_ANI_SMALL_HOLD_RUNNING_LEFT;
					else if (ax == -MARIO_ACCEL_WALK_X)
						aniId = MARIO_ANI_SMALL_HOLD_WALKING_LEFT;
				}
				else {
					if (ax > 0)
						aniId = MARIO_ANI_SMALL_BRAKING_LEFT;
					else if (ax == -MARIO_ACCEL_RUN_X)
						aniId = MARIO_ANI_SMALL_RUNNING_LEFT;
					else if (ax == -MARIO_ACCEL_WALK_X)
						aniId = MARIO_ANI_SMALL_WALKING_LEFT;
				}
				
			}

	if (aniId == -1) aniId = MARIO_ANI_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (isHolding) {
				if (nx >= 0)
					aniId = MARIO_ANI_BIG_HOLD_RUNNING_RIGHT;
				else
					aniId = MARIO_ANI_BIG_HOLD_RUNNING_LEFT;
			}
			else {
				if (nx >= 0)
					aniId = MARIO_ANI_BIG_RUNNING_RIGHT;
				else
					aniId = MARIO_ANI_BIG_RUNNING_LEFT;
			}
			
		}
		else
		{
			if (isHolding) {
				if (nx >= 0)
					aniId = MARIO_ANI_BIG_HOLD_WALKING_FAST_RIGHT;
				else
					aniId = MARIO_ANI_BIG_HOLD_WALKING_FAST_LEFT;
			}
			else {
				if (nx >= 0)
					aniId = MARIO_ANI_BIG_WALKING_FAST_RIGHT;
				else
					aniId = MARIO_ANI_BIG_WALKING_FAST_LEFT;
			}
			
		}
	}
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP) {
		if(isHolding){
			if (nx > 0) {
				aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT;
			}
			if (nx < 0) {
				aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT;
			}
		}
		else {
			if (nx > 0) {
				aniId = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
			}
			if (nx < 0) {
				aniId = MARIO_ANI_BIG_JUMPINGUP_LEFT;
			}
		}
		
	}
	else
		if (isSitting)
		{
			if (nx > 0) {
				aniId = MARIO_ANI_BIG_SITTING_RIGHT;
			}
			else
				aniId = MARIO_ANI_BIG_SITTING_LEFT;
		}
		else
			if (vx == 0)
			{
				if (isHolding) {
					if (nx > 0) {
						aniId = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
					}
					else {
						aniId = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
					}
				}
				else {
					if (nx > 0) {
						aniId = MARIO_ANI_BIG_IDLE_RIGHT;
					}
					else {
						aniId = MARIO_ANI_BIG_IDLE_LEFT;
					}
				}
				
			}
			else if (vx > 0)
			{
				if (isHolding) {
					if (ax < 0)
						aniId = MARIO_ANI_BIG_HOLD_BRAKING_RIGHT;
					else if (ax == MARIO_ACCEL_RUN_X)
					{
						aniId = MARIO_ANI_BIG_HOLD_RUNNING_RIGHT;
					}
					else if (ax == MARIO_ACCEL_WALK_X) {
						aniId = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
					}

					if (!isOnPlatform) {
						aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT;
					}
				}
				else {
					if (ax < 0)
						aniId = MARIO_ANI_BIG_BRAKING_RIGHT;
					else if (ax == MARIO_ACCEL_RUN_X)
					{
						aniId = MARIO_ANI_BIG_RUNNING_RIGHT;
					}
					else if (ax == MARIO_ACCEL_WALK_X) {
						aniId = MARIO_ANI_BIG_WALKING_RIGHT;
					}

					if (!isOnPlatform) {
						aniId = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
					}
				}
				

			}
			else // vx < 0
			{
				if (isHolding) {
					if (ax > 0)
						aniId = MARIO_ANI_BIG_HOLD_BRAKING_LEFT;
					else if (ax == -MARIO_ACCEL_RUN_X) {
						aniId = MARIO_ANI_BIG_HOLD_RUNNING_LEFT;

					}
					else if (ax == -MARIO_ACCEL_WALK_X)
					{
						aniId = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
					}

					if (!isOnPlatform) {
						aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT;
					}
				}
				else {
					if (ax > 0)
						aniId = MARIO_ANI_BIG_BRAKING_LEFT;
					else if (ax == -MARIO_ACCEL_RUN_X) {
						aniId = MARIO_ANI_BIG_RUNNING_LEFT;

					}
					else if (ax == -MARIO_ACCEL_WALK_X)
					{
						aniId = MARIO_ANI_BIG_WALKING_LEFT;
					}

					if (!isOnPlatform) {
						aniId = MARIO_ANI_BIG_JUMPINGUP_LEFT;
					}
				}
				
			}

	if (aniId == -1) aniId = MARIO_ANI_BIG_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	//CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;
	
	if (state == MARIO_STATE_DIE)
		aniId = MARIO_ANI_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();
	animation_set->at(aniId)->Render(x, y);

	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			
			isSitting = true;
			vx = 0; vy = 0.0f;
			
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level == MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH /2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x-MARIO_SMALL_BBOX_WIDTH/2;
		top = y -MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT-MARIO_SMALL_BBOX_HEIGHT)/2;
	}
	level = l;
}

