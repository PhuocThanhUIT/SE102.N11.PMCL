#pragma once
#include "GameObject.h"

#define HIDDEN_BRICK_BBOX_WIDTH  15
#define HIDDEN_BRICK_BBOX_HEIGHT 16

class CHiddenBrick : public CGameObject {
public:
	//int isBlocking = 1;
	virtual void Render();
	virtual int IsBlocking() { return isBlocking; }
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};