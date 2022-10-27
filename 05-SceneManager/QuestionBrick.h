#pragma once
#pragma once
#include "GameObject.h"
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
#define QUESTIONBRICK_PUSH_MAX_HEIGHT 8
#define QUESTION_BRICK_STATE_MARIO_PUSH_UP 100


class CQuestionBrick : public CGameObject
{
protected:

	ULONGLONG push_up_start;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 1; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CQuestionBrick(float x, float y);
	virtual void SetState(int state);
};