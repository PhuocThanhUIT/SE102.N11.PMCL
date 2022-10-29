#include "QuestionBrick.h"
#include "Coin.h"
#include "Mario.h"
#include "PlayScene.h"

CQuestionBrick::CQuestionBrick(float x, float y)
{
}

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BRICK_BBOX_WIDTH;
	b = y + BRICK_BBOX_HEIGHT;
}

void CQuestionBrick::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CQuestionBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//vy += ay * dt;
	//vx += ax * dt;
	if ((state == QUESTION_BRICK_STATE_MARIO_PUSH_UP) && (GetTickCount64() - push_up_start > PUSH_UP_TIMEOUT))
	{
		y+= QUESTIONBRICK_PUSH_MAX_HEIGHT;
		this->SetState(QUESTION_BRICK_STATE_AFTER_MARIO_PUSH_UP);
		this->ShowItem(ITEM_COIN_QUESTION_BRICK_COIN);
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}
void CQuestionBrick::ShowItem(int itemType) {
	this->obj = SetUpItem(itemType);
	if (this->obj == NULL) {
		DebugOut(L"Coin null \n");
		return;
	}
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	if (dynamic_cast<CCoin*>(this->obj)) {
		CCoin* obj = dynamic_cast<CCoin*>(this->obj);
		obj->SetPosition(x, y - COIN_BBOX_HEIGHT - 1);
		currentScene->AddObject(obj);
		DebugOut(L"Coin create \n");
	}
}

CGameObject* CQuestionBrick::SetUpItem(int itemType) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();
	int ani_set_id = -1;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	if (itemType == ITEM_COIN_QUESTION_BRICK_COIN) {
		obj = new CCoin(x,y);
		ani_set_id = COIN_ANI_SET_ID;
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
	}
	return obj;
}


void CQuestionBrick::Render()
{
	int aniId = 0;
	switch (state) {
	case QUESTION_BRICK_STATE_NORMAL:
		aniId = QUESTIONBRICK_ANI_NORMAL;
		break;
	case QUESTION_BRICK_STATE_AFTER_MARIO_PUSH_UP:
		aniId = QUESTIONBRICK_ANI_HIT;
		break;
	}

	animation_set->at(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void CQuestionBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case QUESTION_BRICK_STATE_MARIO_PUSH_UP:
		push_up_start = GetTickCount64();
		y -= QUESTIONBRICK_PUSH_MAX_HEIGHT;
		break;
	}

}
