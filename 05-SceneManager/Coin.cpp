#include "Coin.h"
#include "debug.h"
#include "QuestionBrick.h"
#include "PlayScene.h"


CCoin::CCoin(float x, float y) : CGameObject(x, y) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	mario = currentScene->GetPlayer();
}

void CCoin::Render()
{
	if (isDeleted) return;
	CAnimations* animations = CAnimations::GetInstance();
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH/2;
	t = y - COIN_BBOX_HEIGHT/2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}


void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (tag == COIN_TYPE_INBRICK) {
		if (isDeleted)
			return;
		y += vy * dt;
		x += vx * dt;
		if (tag == COIN_TYPE_INBRICK && vy == 0) {
			this->SetState(COIN_STATE_UP);
		}
		if (start_y - y > COIN_UP_MAX_HEIGH)
		{
			this->SetState(COIN_STATE_DOWN);
		}
		if (start_y - y < COIN_DOWN_LIMIT && vy > 0) {
			mario->AddScore(this->x, this->y, 100);
			this->Delete();
			mario->UpdateCoin();
		}

		CGameObject::Update(dt);
	}
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_UP:
		vy = -COIN_SPEED;
		break;
	case COIN_STATE_DOWN:
		vy = COIN_SPEED;
		break;
	}
}
