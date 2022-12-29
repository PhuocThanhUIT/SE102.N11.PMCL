#include "PiranhaPlantFire.h"
#include "debug.h"
#include "Goomba.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Game.h"
#include "HiddenBrick.h"
CPiranhaPlantFire::CPiranhaPlantFire(float x, float y) :CGameObject(x, y)
{

}

void CPiranhaPlantFire::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - PIRANHA_PLANT_FIRE_BBOX_WIDTH / 2;
	top = y - PIRANHA_PLANT_FIRE_BBOX_HEIGHT / 2;
	right = left + PIRANHA_PLANT_FIRE_BBOX_WIDTH;
	bottom = top + PIRANHA_PLANT_FIRE_BBOX_HEIGHT;
}

void CPiranhaPlantFire::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
};

void CPiranhaPlantFire::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
	}
	else if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = -vx;
	}

}

void CPiranhaPlantFire::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CGameObject::Update(dt, coObjects);
	CCollision::GetInstance()->Process(this, dt, coObjects);
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
	}

}
