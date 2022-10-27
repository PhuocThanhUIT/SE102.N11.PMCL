#include "Goomba.h"
#include "debug.h"
#include "Brick.h"

CGoomba::CGoomba(float x, float y)
{
	this->ax = 0;
	this->ay = GOOMBA_GRAVITY;
	die_start = -1;
	SetState(GOOMBA_STATE_WALKING);
}

void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == GOOMBA_STATE_DIE)
	{
		left = x ;
		top = y ;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT_DIE;
	}
	else
	{ 
		left = x;
		top = y;
		right = left + GOOMBA_BBOX_WIDTH;
		bottom = top + GOOMBA_BBOX_HEIGHT;
	}
}

void CGoomba::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return; 
	if (dynamic_cast<CGoomba*>(e->obj)) return; 
	if (dynamic_cast<CBrick*>(e->obj)) {
		if (e->ny != 0)
		{
			vy = 0;
			if (e->ny < 0 && tag == WING_GOOMBA && state != GOOMBA_STATE_DIE) // check with goomba red fall, not die
			{
				if (GetTickCount64() - walkingTimer >= GOOMBA_WING_TIME_WALKING || !walkingTimer) // jumping
				{
					if (jumpingStacks == GOOMBA_WING_JUMPING_STACKS)
					{
						SetState(GOOMBA_STATE_WING_HIGHJUMPING); // jump
						jumpingStacks = -1; // reset
					}
					else
					{
						if (jumpingStacks == -1) {
							SetState(GOOMBA_STATE_WING_WALKING);
							walkingTimer = GetTickCount64();
							jumpingStacks = 0;
						}
							
						else
						{
							SetState(GOOMBA_STATE_WING_JUMPING);
							jumpingStacks++;
						}
						
					}
				}
			}
		}
	}
	if (e->ny != 0 )
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if ( (state==GOOMBA_STATE_DIE) && (GetTickCount64() - die_start > GOOMBA_DIE_TIMEOUT) )
	{
		isDeleted = true;
		return;
	}

	//// limit y 
	if (vy < -GOOMBA_JUMP_SPEED && state == GOOMBA_STATE_WING_JUMPING)
	{
		vy = -GOOMBA_JUMP_SPEED;
		ay = GOOMBA_GRAVITY;
	}
	if (vy < -GOOMBA_HIGHJUMP_SPEED && state == GOOMBA_STATE_WING_HIGHJUMPING)
	{
		vy = -GOOMBA_HIGHJUMP_SPEED;
		ay = GOOMBA_GRAVITY;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CGoomba::Render()
{
	int aniId = 0;
	switch (tag) {
	case NORMAL_GOOMBA:
		aniId = GOOMBA_NORMAL_ANI_WALKING;
		if (state == GOOMBA_STATE_DIE)
			aniId = GOOMBA_NORMAL_ANI_DIE;
		break;
	case WING_GOOMBA:
		aniId = GOOMBA_WING_ANI_WALKING;
		if (state == GOOMBA_STATE_WING_JUMPING || state == GOOMBA_STATE_WING_HIGHJUMPING)
			aniId = GOOMBA_WING_ANI_JUMPING;
		break;
	case RED_GOOMBA:
		aniId = GOOMBA_RED_ANI_WALKING;
		break;
	}

	animation_set->at(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
		case GOOMBA_STATE_DIE:
			die_start = GetTickCount64();
			y += (GOOMBA_BBOX_HEIGHT - GOOMBA_BBOX_HEIGHT_DIE)/2;
			vx = 0;
			vy = 0;
			ay = 0; 
			break;
		case GOOMBA_STATE_WALKING: 
			vx = -GOOMBA_WALKING_SPEED;
			break;
		case GOOMBA_STATE_WING_WALKING:
			ay = GOOMBA_GRAVITY;
			break;
		case GOOMBA_STATE_WING_JUMPING:
			ay = -GOOMBA_GRAVITY;
			break;
		case GOOMBA_STATE_WING_HIGHJUMPING:
			ay = -GOOMBA_GRAVITY;
			break;
	}
}
