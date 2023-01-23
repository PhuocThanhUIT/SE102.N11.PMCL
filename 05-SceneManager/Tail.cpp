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

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario != NULL) {
		if (x < mario->x)
			nx = -1;
		else
			nx = 1;
		if (mario->nx > 0)
		{
			if (mario->attackStack == 1 || mario->attackStack == 5)
				x = mario->x - TAIL_BBOX_WIDTH;
			else if (mario->attackStack == 3)
				x = mario->x + MARIO_BIG_BBOX_WIDTH;
			else x = mario->x - 8;
		}
		else
		{
			if (mario->attackStack == 1 || mario->attackStack == 5)
				x = mario->x + TAIL_BBOX_WIDTH;
			else if (mario->attackStack == 3)
				x = mario->x - MARIO_BIG_BBOX_WIDTH;
			else x = mario->x + 8;
		}
		y = mario->y + TAIL_BBOX_HEIGHT + 1;
	}

}

void CTail::Render() {
	//RenderBoundingBox();
}