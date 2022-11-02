#include "Coin.h"
#include "debug.h"
#include "QuestionBrick.h"
#include "PlayScene.h"


CCoin::CCoin(float x, float y) : CGameObject(x, y) {
	
}

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}


void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	x += vx * dt;
	if (tag == COIN_TYPE_INBRICK && vy==0) {
		this->SetState(COIN_STATE_UP);
	}
	if (start_y - y > COIN_UP_MAX_HEIGH)
	{
		this->SetState(COIN_STATE_DOWN);
	}
	

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
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
