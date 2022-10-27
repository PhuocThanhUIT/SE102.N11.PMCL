#pragma once
#include "GameObject.h"

#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_DIE_DEFLECT_SPEED	0.35f
#define GOOMBA_JUMP_SPEED			0.125f
#define GOOMBA_HIGHJUMP_SPEED		0.25f

#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 14
#define GOOMBA_BBOX_HEIGHT_DIE 7

#define GOOMBA_DIE_TIMEOUT 500

#define GOOMBA_STATE_WALKING 100
#define GOOMBA_STATE_DIE 200

#define GOOMBA_WING_JUMPING_STACKS	3
#define GOOMBA_WING_TIME_WALKING 500

//ANI
#define GOOMBA_NORMAL_ANI_WALKING		0
#define GOOMBA_NORMAL_ANI_DIE			1
#define GOOMBA_WING_ANI_JUMPING			2
#define GOOMBA_WING_ANI_WALKING		    3
#define GOOMBA_RED_ANI_WALKING			4
#define GOOMBA_RED_ANI_DIE				5

// Red goomba
#define GOOMBA_STATE_DIE_BY_MARIO		250
#define GOOMBA_STATE_WING_JUMPING		300
#define GOOMBA_STATE_WING_HIGHJUMPING	350
#define GOOMBA_STATE_WING_WALKING	400

//Tag
#define NORMAL_GOOMBA 0
#define WING_GOOMBA 1
#define RED_GOOMBA 2


class CGoomba : public CGameObject
{
protected:
	float ax;				
	float ay; 

	int jumpingStacks = 0;

	ULONGLONG die_start;
	ULONGLONG chasingTimer;
	ULONGLONG walkingTimer;

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public: 	
	CGoomba(float x, float y);
	virtual void SetState(int state);
};