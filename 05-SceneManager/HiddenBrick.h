#pragma once
#include "GameObject.h"

#define HIDDEN_BRICK_BBOX_WIDTH  16
#define HIDDEN_BRICK_BBOX_HEIGHT 16

class CHiddenBrick : public CGameObject {
public:
	CHiddenBrick(float x, float y) : CGameObject(x,y) {
		isBlocking = 1;
	}
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking() {
		return isBlocking;
	}

};