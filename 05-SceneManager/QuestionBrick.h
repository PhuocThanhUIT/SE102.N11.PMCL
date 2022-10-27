#pragma once
#pragma once
#include "GameObject.h"
#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16



class CQuestionBrick : public CGameObject
{
protected:


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CQuestionBrick(float x, float y);
	virtual void SetState(int state);
};