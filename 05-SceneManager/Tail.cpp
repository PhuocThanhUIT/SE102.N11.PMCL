#include "Tail.h"
#include "PlayScene.h"

void CTail::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - TAIL_BBOX_WIDTH/2;
	t = y - TAIL_BBOX_HEIGHT/2;
	r = l + TAIL_BBOX_WIDTH;
	b = t + TAIL_BBOX_HEIGHT;
}
//
void CTail::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

	vy += ay * dt;
	vx += ax * dt;

}

void CTail::Render() {
	//RenderBoundingBox();
}