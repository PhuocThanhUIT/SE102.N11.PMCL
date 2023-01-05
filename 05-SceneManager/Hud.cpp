#pragma once
#include "Hud.h"

HUD::HUD(int typeHUD) {

}

void HUD::Render() {
	CSprites::GetInstance()->Get(SPRITE_HUD_ID)->Draw(x, y);
}

void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {

}
