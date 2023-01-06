#pragma once

#include "GameObject.h"
#include "Mario.h"
#define KOOPA_DIFF 7
#define KOOPA_SHELL_DIFF 3
#define KOOPA_STATE_NORMAL 100
#define KOOPA_STATE_SHELL 200
#define KOOPA_STATE_SPIN 300
#define KOOPA_BBOX_WIDTH 16
#define KOOPA_BBOX_HEIGHT 26
#define KOOPA_SHELL_BBOX_WIDTH 14
#define KOOPA_SHELL_BBOX_HEIGHT 16
#define KOOPA_WALK_RIGHT_ANI_ID 0
#define KOOPA_SPIN_RIGHT_ANI_ID 1
#define KOOPA_WALK_LEFT_ANI_ID 2
#define KOOPA_SPIN_LEFT_ANI_ID 3
#define KOOPA_SHELL_ANI_ID 4
#define KOOPA_SHAKE_ANI_ID 5
#define KOOPA_DIE_ANI_ID 6
#define KOOPA_PARA_RIGHT_ANI_ID 7
#define KOOPA_PARA_LEFT_ANI_ID 8
#define KOOPA_SHAKE_UP_ANI_ID 9
#define KOOPA_MOVING_SPEED 0.05f
#define KOOPA_SPIN_SPEED 0.1f
#define KOOPA_GRAVITY 0.02f

#define KOOPA_REVIVE_TIME		2000
#define KOOPA_SHELL_TIME		5000


class CKoopa : public CGameObject
{
public:
	CMario* mario = NULL;
	ULONGLONG reviving_start = 0;
	ULONGLONG shell_start = 0;
	BOOLEAN isBeingHeld = false;
	float ax;
	float ay;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void StartReviving() { reviving_start = GetTickCount64(); }
	void StartShell() { shell_start = GetTickCount64(); reviving_start = 0; }
	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithHiddenBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	int GetAniIdKoopa();
	bool CalTurnableRight(LPGAMEOBJECT object);
	bool CalTurnableLeft(LPGAMEOBJECT object);
	bool SetIsBeingHeld(bool beingheld) { return this->isBeingHeld = beingheld; }
	void HandleBeingHeld(LPGAMEOBJECT object);
	CKoopa(float x, float y);
	virtual void SetState(int state);
};