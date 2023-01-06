#pragma once
#include "GameObject.h"
#include "FireBullet.h"
#define PIRANHAPLANT_GREEN_TYPE					0
#define PIRANHAPLANT_RED_TYPE					1

#define PIRANHAPLANT_BBOX_WIDTH					16
#define PIRANHAPLANT_GREEN_BBOX_HEIGHT			24
#define PIRANHAPLANT_RED_BBOX_HEIGHT			32
#define PIRANHAPLANT_DARTING_SPEED				0.03f
#define PIRANHAPLANT_BBOX_DEATH_WIDTH			16
#define PIRANHAPLANT_BBOX_DEATH_HEIGHT			24

#define PIRANHAPLANT_STATE_DARTING				0
#define PIRANHAPLANT_STATE_SHOOTING				100
#define PIRANHAPLANT_STATE_DEATH				200
#define PIRANHAPLANT_STATE_INACTIVE				300

#define PIRANHAPLANT_ANI_RIGHT_UP			0
#define PIRANHAPLANT_ANI_RIGHT_DOWN			1
#define PIRANHAPLANT_ANI_LEFT_UP			2
#define PIRANHAPLANT_ANI_LEFT_DOWN			3
#define PIRANHAPLANT_ANI_DEATH				4

#define PIRANHAPLANT_DELAY_TIME			750
#define PIRANHAPLANT_DELAY_STOP_TIME	3000
#define PIRANHAPLANT_AIM_TIME			750
#define PIRANHAPLANT_DIYING_TIME		250

#define PIRANHAPLANT_ACTIVE_RANGE		20

#define BULLET_ANI_SET_ID		9



class CPiranhaPlantFire : public CGameObject
{
protected:
	ULONGLONG delay_start = 0;
	ULONGLONG delay_stop = 0;
	ULONGLONG dying_start = 0;
	ULONGLONG shooting_start = 0;
	ULONGLONG aim_start = 0;
	FireBullet* bullet = NULL;
	float limitY = 0;
	bool Up = false;
	bool Right = false;
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	void GetDirect();
	void StartDelay() { delay_start = GetTickCount64(); }
	void StartDelayStop() { delay_stop = GetTickCount64(); }
	void StartShooting() { shooting_start = GetTickCount64(); }
	void StartAim() { aim_start = GetTickCount64(); }
	void Shoot();

public:
	CPiranhaPlantFire(float x, float y);
	virtual void SetState(int state);
};