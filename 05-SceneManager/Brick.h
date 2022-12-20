#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define PLATFORM	1
#define FLASH		12
#define PIPE		10
#define WOOD		46
#define QUESTION	42
#define CLOUDY		26
#define MUSICAL		150

#define BRICK_ANI_IDLE		0

#define BRICK_STATE_IDLE	100



class CBrick : public CGameObject
{
public:
	virtual void Render();
	virtual int IsBlocking() { return 1; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual int IsCollidable() { return 1; };
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};