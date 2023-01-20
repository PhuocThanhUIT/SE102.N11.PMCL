#include "QuestionBrick.h"
#include "Coin.h"
#include "Mario.h"
#include "PlayScene.h"
#include "MushRoom.h"
#include "Leaf.h"
#include "Switch.h"
CQuestionBrick::CQuestionBrick(float x, float y)
{
	this->SetState(QUESTION_BRICK_STATE_NORMAL);
}

void CQuestionBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}



void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y += vy * dt;
	x += vx * dt;
	if ((state == QUESTION_BRICK_STATE_MARIO_PUSH_UP) && (GetTickCount64() - push_up_start > PUSH_UP_TIMEOUT))
	{
		y+= QUESTIONBRICK_PUSH_MAX_HEIGHT;
		this->SetState(QUESTION_BRICK_STATE_AFTER_MARIO_PUSH_UP);
		this->ShowItem(this->tag);
		return;
	}
	// for tail collision
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();
	float mLeft, mTop, mRight, mBottom;
	float oLeft, oTop, oRight, oBottom;
	if (mario != NULL && state != QUESTION_BRICK_STATE_AFTER_MARIO_PUSH_UP) {
		if (mario->isTailAttack && mario->level == MARIO_LEVEL_TAIL) {
			mario->tail->GetBoundingBox(mLeft, mTop, mRight, mBottom);
			GetBoundingBox(oLeft, oTop, oRight, oBottom);
			if (isColliding(floor(mLeft), mTop, ceil(mRight), mBottom))
			{
				SetState(QUESTION_BRICK_STATE_MARIO_PUSH_UP);
				return;
			}
		}
	}
	CGameObject::Update(dt);
}
void CQuestionBrick::ShowItem(int itemType) {
	this->obj = SetUpItem(itemType);
	if (this->obj == NULL) {
		DebugOut(L"Item null \n");
		return;
	}
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	//if (dynamic_cast<CCoin*>(this->obj)) {
		//CCoin* obj = dynamic_cast<CCoin*>(this->obj);
		currentScene->AddObject(this->obj);
	//}
}

CGameObject* CQuestionBrick::SetUpItem(int itemType) {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = currentScene->GetPlayer();
	int ani_set_id = -1;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	switch (itemType)
	{
	case ITEM_COIN_QUESTION_BRICK_COIN:
	{
		obj = new CCoin(x, y - COIN_BBOX_HEIGHT - 1);
		obj->SetTag(COIN_TYPE_INBRICK);
		ani_set_id = COIN_ANI_SET_ID;
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		break;
	}
	case ITEM_MUSHROOM_QUESTION_BRICK:
	{
		obj = new CMushRoom(x,y);
		ani_set_id = MUSHROOM_ANI_SET_ID;
		obj->SetState(MUSHROOM_STATE_IDLE);
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		break;
	}
	case ITEM_LEAF_QUESTION_BRICK:
	{
		obj = new CLeaf(x,y - LEAF_BBOX_HEIGHT);
		obj->SetState(LEAF_STATE_UP);
		ani_set_id = LEAF_ANI_SET_ID;
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		break;
	}
	case ITEM_SWITCH_QUESTION_BRICK:
	{
		obj = new Switch();
		obj->SetState(SWITCH_STATE_UP);
		obj->SetPosition(x, y);
		obj->start_y = y;
		ani_set_id = SWITCH_ANI_SET_ID;
		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
		obj->SetAnimationSet(ani_set);
		break;
	}
	default:
		break;
	}
	
	return obj;
}


void CQuestionBrick::Render()
{
	int aniId = 0;
	
	switch (state) {
	case QUESTION_BRICK_STATE_NORMAL:
		if (this->tag == ITEM_SWITCH_QUESTION_BRICK) aniId = 2;
		else aniId = QUESTIONBRICK_ANI_NORMAL;
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
