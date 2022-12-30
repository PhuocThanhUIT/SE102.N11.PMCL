#include "FireBullet.h"
#include "PiranhaPlantFire.h"
#include "Mario.h"

FireBullet::FireBullet(float bx, float by, bool up, bool right) {
	if (up) {
		y = by;
		vy = -BULLET_SPEED_Y;
	}
	else {
		y = by;
		vy = BULLET_SPEED_Y;
	}

	if (right)
	{
		x = bx;
		vx = BULLET_SPEED_X;
	}
	else
	{
		x = bx;
		vx = -BULLET_SPEED_X;
	}
}

void FireBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	CGameObject::Update(dt);



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	x += vx * dt;
	y += vy * dt;

}

void FireBullet::Render()
{
	int ani = 0;
	if (vx > 0)
		ani = BULLET_ANI_RIGHT;
	else
		ani = BULLET_ANI_LEFT;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void FireBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BULLET_BBOX_WIDTH/2;
	t = y - BULLET_BBOX_HEIGHT/2;
	r = l + BULLET_BBOX_WIDTH;
	b = t + BULLET_BBOX_HEIGHT;
}