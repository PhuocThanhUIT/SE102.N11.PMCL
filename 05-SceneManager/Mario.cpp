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
#include "PiranhaPlant.h"
#include "FireBullet.h"
#include "Leaf.h"
#include "PlayScene.h"
#include "Point.h"

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
	if (abs(vx) > abs(maxVx)) vx = maxVx;

	HandleMarioJump();
	HandleFlying();
	HandleFlapping();
	HandleSpeedStack();

	// reset untouchable timer if untouchable time has passed
	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}


	//isOnPlatform = false;
	DebugOutTitle(L"isOnPlatform:%i", isOnPlatform);
	

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
		OnCollisionWithGoomba(e);
	}
	else if (dynamic_cast<CQuestionBrick*>(e->obj)) {
		OnCollisionWithQuestionBrick(e);
	}
	else if (dynamic_cast<CCoin*>(e->obj)) {
		OnCollisionWithCoin(e);
	}
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CMushRoom*>(e->obj))
		OnCollisionWithMushRoom(e);
	else if (dynamic_cast<CKoopa*>(e->obj)) {
		OnCollisionWithKoopa(e);
	}
	else if (dynamic_cast<CPiranhaPlantFire*>(e->obj))
		OnCollisionWithPiranhaFire(e);
	else if (dynamic_cast<FireBullet*>(e->obj))
		OnCollisionWithFireBullet(e);
	else if (dynamic_cast<CLeaf*>(e->obj))
		OnCollisionWithLeaf(e);
	else if(dynamic_cast<CPiranhaPlant*>(e->obj))
		OnCollisionWithPiranhaFire(e);
}
void CMario::OnCollisionWithLeaf(LPCOLLISIONEVENT e) {
	CLeaf* leaf = dynamic_cast<CLeaf*>(e->obj);
	this->SetLevel(MARIO_LEVEL_TAIL);
	AddScore(leaf->x, leaf->y, 1000);
	leaf->Delete();
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
	AddScore(mushroom->x, mushroom->y, 1000);
	mushroom->SetState(MUSHROOM_STATE_DELETE);
	this->SetLevel(MARIO_LEVEL_BIG);
}
void CMario::OnCollisionWithKoopa(LPCOLLISIONEVENT e)
{
	CKoopa* koopa = dynamic_cast<CKoopa*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (koopa->GetState() == KOOPA_STATE_JUMP) koopa->SetState(KOOPA_STATE_NORMAL);
		else
			koopa->SetState(KOOPA_STATE_SHELL);
			AddScore(koopa->x, koopa->y, 100);
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
				AddScore(goomba->x, goomba->y, 100);
				vy = -MARIO_JUMP_DEFLECT_SPEED;
			}
			else {
				goomba->SetTag(RED_GOOMBA);
				AddScore(goomba->x, goomba->y, 100);
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
	AddScore(e->obj->x, e->obj->y, 100);
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
			if (nx >= 0)
				aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
			else
				aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
			else
				aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;		
		}
	}
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP || isHolding) {
		if (nx > 0) {
			aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
			if (isFlying) {
				aniId = MARIO_ANI_SMALL_FLY_RIGHT;
			}
			if (isHolding) {
				aniId = MARIO_ANI_SMALL_HOLD_RUNNING_RIGHT;
			}
		}
		if (nx < 0) {
			aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
			if (isFlying) {
				aniId = MARIO_ANI_SMALL_FLY_LEFT;
			}
			if (isHolding) {
				aniId = MARIO_ANI_SMALL_HOLD_RUNNING_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = MARIO_ANI_SMALL_IDLE_RIGHT;
			else
				aniId = MARIO_ANI_SMALL_IDLE_LEFT;
		}
		else
			if (vx == 0)
			{	
				if (nx > 0)
					aniId = MARIO_ANI_SMALL_IDLE_RIGHT;
				else
					aniId = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = MARIO_ANI_SMALL_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X) {
					if (isRunning)
						aniId = MARIO_ANI_SMALL_RUNNING_RIGHT;
					else aniId = MARIO_ANI_SMALL_WALKING_RIGHT;
				}
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = MARIO_ANI_SMALL_WALKING_RIGHT;	
				if (!isOnPlatform) {
					aniId = MARIO_ANI_SMALL_JUMPINGUP_RIGHT;
					if (isFlying) {
						aniId = MARIO_ANI_SMALL_FLY_RIGHT;
					}
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = MARIO_ANI_SMALL_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
				{
					if (isRunning)
						aniId = MARIO_ANI_SMALL_RUNNING_LEFT;
					else aniId = MARIO_ANI_SMALL_WALKING_LEFT;
				}
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = MARIO_ANI_SMALL_WALKING_LEFT;
				if (!isOnPlatform) {
					aniId = MARIO_ANI_SMALL_JUMPINGUP_LEFT;
					if (isFlying) {
						aniId = MARIO_ANI_SMALL_FLY_LEFT;
					}
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
			if (nx >= 0)
				aniId = MARIO_ANI_BIG_RUNNING_RIGHT;
			else
				aniId = MARIO_ANI_BIG_RUNNING_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = MARIO_ANI_BIG_WALKING_FAST_RIGHT;
			else
				aniId = MARIO_ANI_BIG_WALKING_FAST_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP) {
		if (nx > 0) {
			aniId = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
			if (isHolding) {
				aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT;
			}
		}
		if (nx < 0) {
			aniId = MARIO_ANI_BIG_JUMPINGUP_LEFT;
			if (isHolding) {
				aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT;
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
				if (nx > 0) {
					aniId = MARIO_ANI_BIG_IDLE_RIGHT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_IDLE_RIGHT;
					}
				}
				else {
					aniId = MARIO_ANI_BIG_IDLE_LEFT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_IDLE_LEFT;
					}
				}
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = MARIO_ANI_BIG_BRAKING_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
				{
					if (isRunning) {
						aniId = MARIO_ANI_BIG_RUNNING_RIGHT;
					}else aniId = MARIO_ANI_BIG_WALKING_RIGHT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_RUNNING_RIGHT;
					}
				}
				else if (ax == MARIO_ACCEL_WALK_X) {
					aniId = MARIO_ANI_BIG_WALKING_RIGHT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_WALKING_RIGHT;
					}
				}

				if (!isOnPlatform) {
						aniId = MARIO_ANI_BIG_JUMPINGUP_RIGHT;
						if (isFlying) {
							aniId = MARIO_ANI_BIG_FLY_RIGHT;
						}
						if (isHolding) {
							aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT;
						}
				}
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = MARIO_ANI_BIG_BRAKING_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X) {
					if (isRunning) {
						aniId = MARIO_ANI_BIG_RUNNING_LEFT;
					}
					else aniId = MARIO_ANI_BIG_WALKING_LEFT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_RUNNING_LEFT;
					}
				}
				else if (ax == -MARIO_ACCEL_WALK_X)
				{
					aniId = MARIO_ANI_BIG_WALKING_LEFT;
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_WALKING_LEFT;
					}
				}
				if (!isOnPlatform) {
					aniId = MARIO_ANI_BIG_JUMPINGUP_LEFT;
					if (isFlying) {
						aniId = MARIO_ANI_BIG_FLY_LEFT;
					}
					if (isHolding) {
						aniId = MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT;
					}
				}
			}

	if (aniId == -1) aniId = MARIO_ANI_BIG_IDLE_RIGHT;

	return aniId;
}

int CMario::GetAniIdTail()
{
	int aniId = -1;
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = MARIO_ANI_TAIL_RUNNING_RIGHT;
			else
				aniId = MARIO_ANI_TAIL_RUNNING_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = MARIO_ANI_TAIL_WALKING_FAST_RIGHT;
			else
				aniId = MARIO_ANI_TAIL_WALKING_FAST_LEFT;
		}
	}
	if (state == MARIO_STATE_JUMP || state == MARIO_STATE_RELEASE_JUMP || isHolding) {
		if (nx > 0) {
			aniId = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
			if (isTailFlying) {
				aniId = MARIO_ANI_TAIL_FLY_UP_RIGHT;
			}
			if (isFlapping) {
				aniId = MARIO_ANI_TAIL_FLAPPING_RIGHT;
			}
			else if (isHolding) {
				aniId = MARIO_ANI_TAIL_HOLD_RUNNING_RIGHT;
			}
		}
		if (nx < 0) {
			aniId = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
			if (isTailFlying) {
				aniId = MARIO_ANI_TAIL_FLY_UP_LEFT;
			}
			if (isFlapping)
				aniId = MARIO_ANI_TAIL_FLAPPING_LEFT;
			else if (isHolding) {
				aniId = MARIO_ANI_TAIL_HOLD_RUNNING_LEFT;
			}
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0) {
				aniId = MARIO_ANI_TAIL_SITTING_RIGHT;
			}
			else
				aniId = MARIO_ANI_TAIL_SITTING_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) {
					aniId = MARIO_ANI_TAIL_IDLE_RIGHT;
				}
				else {
					aniId = MARIO_ANI_TAIL_IDLE_LEFT;
				}
			}
			else if (vx > 0)
			{
					if (ax < 0)
						aniId = MARIO_ANI_TAIL_BRAKING_RIGHT;
					else if (ax == MARIO_ACCEL_RUN_X)
					{
						if (isRunning) {
							aniId = MARIO_ANI_TAIL_RUNNING_RIGHT;
						} else aniId = MARIO_ANI_TAIL_WALKING_RIGHT;
						
					}
					else if (ax == MARIO_ACCEL_WALK_X) {
						aniId = MARIO_ANI_TAIL_WALKING_RIGHT;
					}

					if (!isOnPlatform) {
						aniId = MARIO_ANI_TAIL_JUMPINGUP_RIGHT;
						if (isTailFlying) {
							if (isTailFlyFlapping) aniId = MARIO_ANI_TAIL_FLY_FLAPPING_RIGHT;
							else aniId = MARIO_ANI_TAIL_FLY_UP_RIGHT;
						}
						if (isFlapping)
							aniId = MARIO_ANI_TAIL_FLAPPING_RIGHT;
					}
			}
			else // vx < 0
			{
				
					if (ax > 0)
						aniId = MARIO_ANI_TAIL_BRAKING_LEFT;
					else if (ax == -MARIO_ACCEL_RUN_X) {
						if (isRunning) {
							aniId = MARIO_ANI_TAIL_RUNNING_LEFT;
						}
						else aniId = MARIO_ANI_TAIL_WALKING_LEFT;
					}
					else if (ax == -MARIO_ACCEL_WALK_X)
					{
						aniId = MARIO_ANI_TAIL_WALKING_LEFT;
					}

					if (!isOnPlatform) {
						aniId = MARIO_ANI_TAIL_JUMPINGUP_LEFT;
						if (isTailFlying) {
							if (isTailFlyFlapping) aniId = MARIO_ANI_TAIL_FLY_FLAPPING_LEFT;
							else aniId = MARIO_ANI_TAIL_FLY_UP_LEFT;
						}
						if (isFlapping)
							aniId = MARIO_ANI_TAIL_FLAPPING_LEFT;
					}

			}

	if (aniId == -1) aniId = MARIO_ANI_TAIL_IDLE_RIGHT;

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
	else if (level == MARIO_LEVEL_TAIL)
		aniId = GetAniIdTail();
	animation_set->at(aniId)->Render(x, y);

	
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
		isReadyToRun = true;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		isReadyToRun = true;
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
			isJumping = true;
		}
		if (abs(ax) == MARIO_ACCEL_RUN_X) {
			if (level == MARIO_LEVEL_TAIL && isRunning) {
				isTailFlying = true;
				isOnPlatform = false;
				StartTailFlying();
			}
			else if (isRunning) {
				isFlying = true;
				isOnPlatform = false;
				StartFlying();
			}
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
	if (level != MARIO_LEVEL_SMALL)
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

void CMario::HandleFlying() {
	if (level != -5) {
		if (isTailFlying||isFlying)
		{
			if (GetTickCount64() - tail_fly_min_start < MARIO_FLYING_TIME_MIN) {
					ay = 0.00f;
					isTailFlyFlapping = true;
			}
			else {
				if (vy <= -MARIO_FLY_MAX) {
					isTailFlyFlapping = false;
					ay = 0.001f;
				}
			}
		}
		else {
			ay = MARIO_GRAVITY;
		}
	}
	if (GetTickCount64() - tail_fly_start > MARIO_FLYING_TIME && tail_fly_start != 0 && isTailFlying)
	{
		tail_fly_start = 0;
		isTailFlying = false;
	}
	if (GetTickCount64() - fly_start > MARIO_FLYING_TIME && fly_start != 0 && isFlying)
	{
		fly_start = 0;
		isFlying = false;
	}
}

void CMario::HandleFlapping() {
	if (level == MARIO_LEVEL_TAIL && isFlapping) {
		vy = MARIO_SLOW_FALLING_SPEED;
	}
}
void CMario::HandleMarioJump() {
	if (isJumping) {
		isOnPlatform = false;
		if (abs(this->vx) == MARIO_RUNNING_SPEED) {
			vy = -MARIO_JUMP_RUN_SPEED_Y;
			ay = MARIO_GRAVITY; isJumping = false;
		}
		else {
			vy = -MARIO_JUMP_SPEED_Y;
			ay = MARIO_GRAVITY; isJumping = false;
		}

	}
}

void CMario::HandleSpeedStack() {
	if (GetTickCount64() - start_running > MARIO_RUNNING_STACK_TIME && vx != 0 && isReadyToRun && (!isFlying&&!isTailFlying||isOnPlatform)) {
		start_running = GetTickCount64();
		speedStack++;
		if (speedStack >= MARIO_RUNNING_STACKS) {
			isRunning = true;
			speedStack = MARIO_RUNNING_STACKS;
		}
	}
	if (GetTickCount64() - start_running > MARIO_RUNNING_STACK_TIME && (!isReadyToRun || vx == 0||isFlying||isTailFlying))
	{
		isRunning = false;
		start_running = GetTickCount64();
		speedStack--;
		if (speedStack < 0)
		{
			speedStack = 0;
		}
	}
	//if (GetTickCount64() - start_running > MARIO_RUNNING_STACK_TIME && isFlying && isTailFlying)
	//{
	//	isRunning = false;
	//	isFlying = false;
	//	isTailFlying = false;
	//	start_running = GetTickCount64();
	//	speedStack--;
	//	//isFlying = false;
	//	if (speedStack < 0)
	//	{
	//		speedStack = 0;
	//	}
	//}
}

void CMario::AddScore(float x, float y, int score) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	Point* point = new Point(score);
	int previousScore = score;

	point->SetPosition(x, y);
	currentScene->AddObject(point);

	this->marioScore += score;

}
