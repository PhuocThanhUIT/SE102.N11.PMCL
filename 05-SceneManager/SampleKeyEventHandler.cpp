#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer(); 

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (mario->isTailFlying && mario->level==MARIO_LEVEL_TAIL && mario->y>30) {
			if (mario->isOnPlatform) {
				mario->vy = -0.035f;
			}
			mario->ay = -0.0005f;
			mario->StartMinTailFlying();
		}
		else if (mario->level == MARIO_LEVEL_TAIL && !mario->isOnPlatform)
			mario->isFlapping = true;
		else {
			if(mario->isOnPlatform)
			mario->SetState(MARIO_STATE_JUMP);
		}
		break;
	case DIK_A:
		if (mario->level == MARIO_LEVEL_TAIL) {
			mario->StartAttack();
		}
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_TAIL);
		break;
	case DIK_4:
		mario->HackPosition();
		break;
	case DIK_5:
		mario->StartExtraMap();
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_R: // reset
		//Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (KeyCode)
	{
	case DIK_S:
		mario->isFlapping = false;
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_A:
		if (mario->isHolding) {
			mario->isReadyToHold = false;
			mario->isHolding = false;
		}
		mario->isReadyToRun = false;
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A)) {
			mario->isReadyToHold = true;
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		}
		else
		{
			mario->isReadyToHold = false;
			mario->isHolding = false;
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
		}	
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A)) {
			mario->isReadyToHold = true;
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		}
		else {
			mario->isReadyToHold = false;
			mario->isHolding = false;
			mario->SetState(MARIO_STATE_WALKING_LEFT);
		}
			
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}