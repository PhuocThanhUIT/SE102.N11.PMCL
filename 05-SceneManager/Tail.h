#pragma once
#include "GameObject.h"

#define TAIL_BBOX_WIDTH		12
#define TAIL_BBOX_HEIGHT	6

class CTail :public CGameObject
{
	float ax;
	float ay;
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	CTail(float x, float y) { this->x = x; this->y = y; }
	virtual int IsBlocking() { return 0; }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};