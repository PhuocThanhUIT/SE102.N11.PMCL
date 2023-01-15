#include "PiranhaPlantFire.h"
#include "debug.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"
#include "HiddenBrick.h"
CPiranhaPlantFire::CPiranhaPlantFire(float x, float y)
{
	if (tag == PIRANHAPLANT_GREEN_TYPE) {
		this->limitY = y - PIRANHAPLANT_GREEN_BBOX_HEIGHT;
		this->BBHeight = PIRANHAPLANT_GREEN_BBOX_HEIGHT;
	}
	else {
		this->limitY = y - PIRANHAPLANT_RED_BBOX_HEIGHT;
		this->BBHeight = PIRANHAPLANT_RED_BBOX_HEIGHT;
	}
	SetState(PIRANHAPLANT_STATE_DARTING);
}

void CPiranhaPlantFire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (tag == PIRANHAPLANT_GREEN_TYPE) {
		left = x - PIRANHAPLANT_BBOX_WIDTH / 2;
		top = y - PIRANHAPLANT_GREEN_BBOX_HEIGHT / 2;
		right = left + PIRANHAPLANT_BBOX_WIDTH;
		bottom = top + PIRANHAPLANT_GREEN_BBOX_HEIGHT;
	}
	else {
		left = x - PIRANHAPLANT_BBOX_WIDTH / 2;
		top = y - PIRANHAPLANT_RED_BBOX_HEIGHT / 2;
		right = left + PIRANHAPLANT_BBOX_WIDTH;
		bottom = top + PIRANHAPLANT_RED_BBOX_HEIGHT;
	}
	
}


void CPiranhaPlantFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt);
	y += vy * dt;
	GetDirect();
	if (y <= limitY && vy < 0)
	{
		y = limitY;
		vy = 0;
		StartAim();
	}
	if (y >= limitY + BBHeight && vy > 0) {
		vy = 0;
		y = limitY + BBHeight + 12;
	}

	if (GetTickCount64() - delay_stop >= PIRANHAPLANT_DELAY_STOP_TIME && delay_stop != 0)
	{
		delay_stop = 0;
		SetState(PIRANHAPLANT_STATE_DARTING);
		//StartDelay();
	}

	if (GetTickCount64() - aim_start >= PIRANHAPLANT_AIM_TIME && aim_start != 0)
	{
		aim_start = 0;
		SetState(PIRANHAPLANT_STATE_SHOOTING);
		StartDelay();
	}

	if (GetTickCount64() - delay_start >= PIRANHAPLANT_DELAY_TIME && delay_start != 0)
	{
		delay_start = 0;
		if (y == limitY)
			vy = PIRANHAPLANT_DARTING_SPEED;
		StartDelayStop();
	}
}
void CPiranhaPlantFire::GetDirect() {
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int mHeight;
	if (mario->level == MARIO_LEVEL_SMALL)
		mHeight = MARIO_SMALL_BBOX_HEIGHT;
	else
		mHeight = MARIO_BIG_BBOX_HEIGHT;

	if (mario->y + mHeight < limitY + BBHeight)
		Up = true;
	else
		Up = false;
	if (mario->x <= x)
		Right = false;
	else
		Right = true;
};
void CPiranhaPlantFire::Shoot() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	this->bullet = new FireBullet(x, y, Up, Right);

	//! Basic setup for bullet object
	int ani_set_id = BULLET_ANI_SET_ID;
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	bullet->SetAnimationSet(ani_set);
	currentScene->AddObjectEnd(bullet);
}

void CPiranhaPlantFire::Render()
{
	int ani = PIRANHAPLANT_ANI_DEATH;
	if (state != PIRANHAPLANT_STATE_DEATH && dying_start == 0)
	{
		if (Up)
			if (Right)
				ani = PIRANHAPLANT_ANI_RIGHT_UP;
			else
				ani = PIRANHAPLANT_ANI_LEFT_UP;
		else
			if (Right)
				ani = PIRANHAPLANT_ANI_RIGHT_DOWN;
			else
				ani = PIRANHAPLANT_ANI_LEFT_DOWN;
	}
	animation_set->at(ani)->Render(x, y);
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
	case PIRANHAPLANT_STATE_SHOOTING:
		vy = 0;
		Shoot();
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		break;
	}

}
