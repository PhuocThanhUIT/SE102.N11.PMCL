#include "QuestionBrick.h"


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
		this->SetState(200);
		return;
	}

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
}


void CQuestionBrick::Render()
{
	int aniId = 0;

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
