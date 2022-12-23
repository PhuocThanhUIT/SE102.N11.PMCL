#include "Brick.h"
#include "Utils.h"
#include "Koopa.h"

void CBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH/2;
	t = y - BRICK_BBOX_HEIGHT/2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;

}
//void CBrick::OnCollisionWith(LPCOLLISIONEVENT e)
//{
//	if (dynamic_cast<CKoopa*>(e->obj)) {
//		CKoopa *koopa = dynamic_cast<CKoopa*>(e->obj);
//		if (e->nx != 0) koopa->vx = -koopa->vx;
//	}
//	
//}
//void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
//{
//
//	CGameObject::Update(dt, coObjects);
//	CCollision::GetInstance()->Process(this, dt, coObjects);
//}
void CBrick::Render()
{
	animation_set->at(BRICK_ANI_IDLE)->Render(x, y);
	//RenderBoundingBox();
}
