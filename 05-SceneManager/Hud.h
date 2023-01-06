#pragma once
#include "GameObject.h"
#include "Mario.h"

#define FONT_BBOX_WIDTH						8
#define DEFAULT_TIME						300

#define SPRITE_HUD_ID						99999
#define SPRITE_ICONMARIO_ID					50003
#define SPRITE_ICONLUIGI_ID					50004
#define SPRITE_FILLARROW_ID					50005
#define ANI_P_ID							5006

class HUD :public CGameObject {

public:
	CMario* mario;
	LPSPRITE playerSprite = NULL;
	vector<LPSPRITE> powerMelterSprite;
	int speedStack = 0;
	LPANIMATION PAni = NULL;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	HUD(int type_hud = 0);
	virtual void Render();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	void AddSpeedStack();
};