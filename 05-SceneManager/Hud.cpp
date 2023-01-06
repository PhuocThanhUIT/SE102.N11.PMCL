#pragma once
#include "Hud.h"
#include <string>
#include "PlayScene.h"

#define HUD_DIFF_P				15
#define HUD_DIFF_ROW			4	
#define HUD_DIFF_METTER			68

HUD::HUD(int typeHUD) {
	playerSprite = CSprites::GetInstance()->Get(SPRITE_ICONMARIO_ID);
	PAni = CAnimations::GetInstance()->Get(ANI_P_ID);
	for (unsigned int i = 0; i < MARIO_RUNNING_STACKS - 1; i++)
		powerMelterSprite.push_back((CSprites::GetInstance()->Get(SPRITE_FILLARROW_ID)));
}

void HUD::Render() {
	CSprites::GetInstance()->Get(SPRITE_HUD_ID)->Draw(x, y);

	for (int i = 1; i <= speedStack; i++) {
		if (i == MARIO_RUNNING_STACKS) {
			if (PAni != nullptr)
				PAni->Render(x - HUD_DIFF_P, y - HUD_DIFF_ROW);
		}
		else
		{
			powerMelterSprite[i - 1]->Draw(x + FONT_BBOX_WIDTH * (i - 1) - HUD_DIFF_METTER, y - 4);
		}
	}
}

void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	AddSpeedStack();
}

void HUD::AddSpeedStack() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	mario = currentScene->GetPlayer();
	this->speedStack = mario->speedStack;
}