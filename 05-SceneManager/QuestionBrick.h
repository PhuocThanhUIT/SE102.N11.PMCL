#pragma once

#include "GameObject.h"
#define QUESTIONBRICK_ANI_NORMAL 0
#define QUESTIONBRICK_ANI_HIT 1
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16
#define QUESTIONBRICK_PUSH_MAX_HEIGHT 8
#define QUESTION_BRICK_STATE_NORMAL 100
#define QUESTION_BRICK_STATE_MARIO_PUSH_UP 200
#define QUESTION_BRICK_STATE_AFTER_MARIO_PUSH_UP 300
#define PUSH_UP_TIMEOUT 100
#define ITEM_COIN_QUESTION_BRICK_COIN	0
#define COIN_ANI_SET_ID 6
#define ITEM_MUSHROOM_QUESTION_BRICK 1
#define MUSHROOM_ANI_SET_ID 37


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
	CGameObject* obj = NULL;
	CGameObject* SetUpItem(int itemType);
	void ShowItem(int itemType = ITEM_COIN_QUESTION_BRICK_COIN);
	CQuestionBrick(float x, float y);
	virtual void SetState(int state);
};