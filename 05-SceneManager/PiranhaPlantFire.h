#pragma once
#pragma once
#pragma once
#include "GameObject.h"
#define PIRANHA_PLANT_FIRE_BBOX_WIDTH 16
#define PIRANHA_PLANT_FIRE_BBOX_HEIGHT 32



class CPiranhaPlantFire : public CGameObject
{
protected:
	float start_x = this->x;
	float start_y = this->y;
	float ay;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CPiranhaPlantFire(float x, float y);
	virtual void SetState(int state);
};