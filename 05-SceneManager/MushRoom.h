#pragma once
#pragma once
#include "GameObject.h"
#define MUSHROOM_BBOX_WIDTH 16
#define MUSHROOM_BBOX_HEIGHT 16
#define MUSHROOM_STATE_IDLE 100
#define MUSHROOM_STATE_MOVE 200
#define MUSHROOM_MOVING_SPEED 0.15f
#define MUSHROOM_GROWING_UP_SPEED 0.05f
#define MUSHROOM_GRAVITY 0.002f

class CMushRoom : public CGameObject
{
protected:
	float ax;
	float ay;


	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMushRoom(float x, float y);
	virtual void SetState(int state);
};