#pragma once
#include "GameObject.h"

#define LEAF_BBOX_WIDTH  16
#define LEAF_BBOX_HEIGHT 16

#define LEAF_ANI_SET_ID		36

#define LEAF_ANI_RIGHT		0
#define LEAF_ANI_LEFT		1

class CLeaf : public CGameObject
{
protected:
	float start_x = this->x;
	float start_y = this->y;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);


public:
	virtual void SetState(int state);
};