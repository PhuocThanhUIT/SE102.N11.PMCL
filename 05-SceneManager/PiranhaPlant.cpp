#include "PiranhaPlant.h"
#include "Game.h"
#include "PlayScene.h"

void CPiranhaPlant::GetBoundingBox(float& left, float& top,
	float& right, float& bottom)
{
	left = x - PIRANHA_BBOX_WIDTH/2;
	top = y - PIRANHAPLANT_BBOX_HEIGHT/2;
	right = left + PIRANHA_BBOX_WIDTH;
	bottom = top + PIRANHAPLANT_BBOX_HEIGHT;
}

CPiranhaPlant::CPiranhaPlant(float x, float y)
{
	limitY = y - PIRANHAPLANT_BBOX_HEIGHT_ANI;
	SetState(PIRANHAPLANT_STATE_IDLE);
}

void CPiranhaPlant::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt, coObjects);


	if (GetTickCount64() - dying_start >= PIRANHAPLANT_DIYING_TIME && dying_start != 0)
		this->Delete();

	if (y <= limitY && vy < 0)
	{
		y = limitY;
		SetState(PIRANHAPLANT_STATE_BITING);
	}
	if (y >= limitY + PIRANHAPLANT_BBOX_HEIGHT && vy > 0)
	{
		y = limitY + PIRANHAPLANT_BBOX_HEIGHT;
	}
	if (GetTickCount64() - delay_stop >= PIRANHAPLANT_DELAY_STOP_TIME && delay_stop != 0) {
		SetState(PIRANHAPLANT_STATE_IDLE);
		delay_stop = 0;
	}
	if (GetTickCount64() - biting_start >= PIRANHAPLANT_BITING_TIME && biting_start != 0)
	{
		if (y == limitY)
			vy = PIRANHAPLANT_DARTING_SPEED;
		biting_start = 0;
		StartDelayStop();
	}

	//x += vx * dt;
	y += vy * dt;
	// for tail collision
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	if (mario != NULL && state != PIRANHAPLANT_STATE_DEATH) {
		if (mario->isTailAttack && mario->level == MARIO_LEVEL_TAIL) {
			mario->tail->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom))
			{
				SetState(PIRANHAPLANT_STATE_DEATH);
				mario->AddScore(x, y, 100);
			}
		}
	}
	

}

void CPiranhaPlant::Render()
{
	int ani = PIRANHA_ANI_DEATH;
	if (state != PIRANHAPLANT_STATE_DEATH && dying_start == 0)
		ani = PIRANHAPLANT_ANI_IDLE;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CPiranhaPlant::SetState(int _state)
{
	CGameObject::SetState(_state);
	switch (_state)
	{
	case PIRANHAPLANT_STATE_IDLE:
		//DebugOut(L"pira state \n");
		vy = -PIRANHAPLANT_DARTING_SPEED;
		break;
	case PIRANHAPLANT_STATE_BITING:
		vy = 0;
		StartBitting();
		break;
	case PIRANHAPLANT_STATE_INACTIVE:
		vy = 0;
		StartBitting();
		break;
	case PIRANHAPLANT_STATE_DEATH:
		vy = 0;
		break;
	}
}