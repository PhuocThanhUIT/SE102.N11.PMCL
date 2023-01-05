#pragma once
#include "GameObject.h"
#include "Mario.h"

#define FONT_BBOX_WIDTH						8
#define DEFAULT_TIME						300

#define SPRITE_HUD_ID						99999

class HUD :public CGameObject {

public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	HUD(int type_hud = 0);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
};