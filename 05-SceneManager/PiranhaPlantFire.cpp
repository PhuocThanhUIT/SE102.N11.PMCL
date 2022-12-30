#include "PiranhaPlantFire.h"
#include "debug.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"
#include "HiddenBrick.h"
CPiranhaPlantFire::CPiranhaPlantFire(float x,float y)
{
	this->limitY = y - PIRANHAPLANT_RED_BBOX_HEIGHT;
	SetState(PIRANHAPLANT_STATE_DARTING);
}

void CPiranhaPlantFire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PIRANHAPLANT_BBOX_WIDTH / 2;
	top = y - PIRANHAPLANT_RED_BBOX_HEIGHT / 2;
	right = left + PIRANHAPLANT_BBOX_WIDTH;
	bottom = top + PIRANHAPLANT_RED_BBOX_HEIGHT;
}


void CPiranhaPlantFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt);
	y += vy * dt;

	if (y <= limitY && vy < 0)
	{
		y = limitY;
		vy = 0;
		StartDelay();
	}

	if (y >= limitY + PIRANHAPLANT_RED_BBOX_HEIGHT && vy > 0)
	{
		y = limitY + PIRANHAPLANT_RED_BBOX_HEIGHT + 12;
		SetState(PIRANHAPLANT_STATE_DARTING);
		StartDelay();
	}

	if (GetTickCount64() - delay_start >= PIRANHAPLANT_DELAY_TIME && delay_start != 0)
	{
		delay_start = 0;
		if (y == limitY)
			vy = PIRANHAPLANT_DARTING_SPEED;
	}
}


void CPiranhaPlantFire::Render()
{
	if (isDeleted) return;
	int aniId = 0;
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CPiranhaPlantFire::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case PIRANHAPLANT_STATE_DARTING:
		vy = -PIRANHAPLANT_DARTING_SPEED;
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		break;
	}

}
