#pragma once

#include "GameObject.h"
#include "Animations.h"
#include "Mario.h"

#define ID_ANI_NORMAL_COIN 0

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_TYPE_INBRICK	61
#define COIN_STATE_UP 0
#define COIN_STATE_DOWN 1
#define COIN_SPEED 0.15f
#define COIN_UP_MAX_HEIGH 30
#define COIN_DOWN_LIMIT 5


class CCoin : public CGameObject {
public:
	CCoin(float x, float y);
	CMario* mario = NULL;
	float start_x = this->x;
	float start_y = this->y;
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return 0; }
	virtual void SetState(int state);
};