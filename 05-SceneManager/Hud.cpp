#pragma once
#include "Hud.h"
#include <string>
#include "PlayScene.h"
#include "WorldScene.h"
#include "IntroScene.h"

#define HUD_DIFF_P				15
#define HUD_DIFF_ROW			4	
#define HUD_DIFF_METTER			68
#define HUD_DIFF_FIRST_ROW		8
#define HUD_DIFF_SECOND_ROW		16
#define HUD_DIFF_PLAYER			110
#define HUD_DIFF_TIME			5
#define HUD_DIFF_MONEY			12
#define HUD_DIFF_LIFE			93
#define HUD_DIFF_SCORE			60

#define HUD_TIME_MAX	3
#define HUD_SCORE_MAX	7

#define HUD_HEIGHT 32

void HUD::initFonts() {
	CSprites* sprites = CSprites::GetInstance();
	fonts.insert(make_pair('0', sprites->Get(SPRITE_FONT_0_ID)));
	fonts.insert(make_pair('1', sprites->Get(SPRITE_FONT_1_ID)));
	fonts.insert(make_pair('2', sprites->Get(SPRITE_FONT_2_ID)));
	fonts.insert(make_pair('3', sprites->Get(SPRITE_FONT_3_ID)));
	fonts.insert(make_pair('4', sprites->Get(SPRITE_FONT_4_ID)));
	fonts.insert(make_pair('5', sprites->Get(SPRITE_FONT_5_ID)));
	fonts.insert(make_pair('6', sprites->Get(SPRITE_FONT_6_ID)));
	fonts.insert(make_pair('7', sprites->Get(SPRITE_FONT_7_ID)));
	fonts.insert(make_pair('8', sprites->Get(SPRITE_FONT_8_ID)));
	fonts.insert(make_pair('9', sprites->Get(SPRITE_FONT_9_ID)));
	fonts.insert(make_pair('A', sprites->Get(SPRITE_FONT_A_ID)));
	fonts.insert(make_pair('B', sprites->Get(SPRITE_FONT_B_ID)));
	fonts.insert(make_pair('C', sprites->Get(SPRITE_FONT_C_ID)));
	fonts.insert(make_pair('D', sprites->Get(SPRITE_FONT_D_ID)));
	fonts.insert(make_pair('E', sprites->Get(SPRITE_FONT_E_ID)));
	fonts.insert(make_pair('F', sprites->Get(SPRITE_FONT_F_ID)));
	fonts.insert(make_pair('G', sprites->Get(SPRITE_FONT_G_ID)));
	fonts.insert(make_pair('H', sprites->Get(SPRITE_FONT_H_ID)));
	fonts.insert(make_pair('I', sprites->Get(SPRITE_FONT_I_ID)));
	fonts.insert(make_pair('J', sprites->Get(SPRITE_FONT_J_ID)));
	fonts.insert(make_pair('K', sprites->Get(SPRITE_FONT_K_ID)));
	fonts.insert(make_pair('L', sprites->Get(SPRITE_FONT_L_ID)));
	fonts.insert(make_pair('M', sprites->Get(SPRITE_FONT_M_ID)));
	fonts.insert(make_pair('N', sprites->Get(SPRITE_FONT_N_ID)));
	fonts.insert(make_pair('O', sprites->Get(SPRITE_FONT_O_ID)));
	fonts.insert(make_pair('P', sprites->Get(SPRITE_FONT_P_ID)));
	fonts.insert(make_pair('Q', sprites->Get(SPRITE_FONT_Q_ID)));
	fonts.insert(make_pair('R', sprites->Get(SPRITE_FONT_R_ID)));
	fonts.insert(make_pair('S', sprites->Get(SPRITE_FONT_S_ID)));
	fonts.insert(make_pair('T', sprites->Get(SPRITE_FONT_T_ID)));
	fonts.insert(make_pair('U', sprites->Get(SPRITE_FONT_U_ID)));
	fonts.insert(make_pair('V', sprites->Get(SPRITE_FONT_V_ID)));
	fonts.insert(make_pair('W', sprites->Get(SPRITE_FONT_W_ID)));
	fonts.insert(make_pair('X', sprites->Get(SPRITE_FONT_X_ID)));
	fonts.insert(make_pair('Y', sprites->Get(SPRITE_FONT_Y_ID)));
	fonts.insert(make_pair('Z', sprites->Get(SPRITE_FONT_Z_ID)));
}

LPSPRITE HUD::mappingFont(char c) {
	int ic = (int)c;
	if (!(ic >= 48 && ic <= 57 || ic >= 64 && ic <= 90))
		return NULL;
	return fonts.at(c);
}

vector<LPSPRITE> HUD::StringToSprite(string str)
{
	vector<LPSPRITE> sprites;
	LPSPRITE sprite;
	char c;
	for (unsigned int i = 0; i < str.size(); i++)
	{
		c = (char)str[i];
		sprite = mappingFont(c);
		if (sprite != NULL)
			sprites.push_back(sprite);
	}
	return sprites;
}

HUD::HUD(int typeHUD) {
	this->typeHud = typeHUD;
	initFonts();
	playerSprite = CSprites::GetInstance()->Get(SPRITE_ICONMARIO_ID);
	PAni = CAnimations::GetInstance()->Get(ANI_P_ID);
	int sceneId = CGame::GetInstance()->GetCurrentScene()->GetSceneId();
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	if (typeHUD == 1) {
		this->marioLife = 4;
		this->score = 0;
		this->money = 0;
		mariolifeSprites = StringToSprite(to_string(marioLife));
		moneySprites = StringToSprite(to_string(money));
		string score_str = to_string(score);
		while (score_str.length() < HUD_SCORE_MAX) score_str = "0" + score_str;
		scoreSprites = StringToSprite(score_str);
		string time_str = to_string(DEFAULT_TIME);
		while (time_str.length() < HUD_TIME_MAX) time_str = "0" + time_str;
		remainTimeSprites = StringToSprite(time_str);
	}
	else {
		LoadBackUpHud();
		for (unsigned int i = 0; i < MARIO_RUNNING_STACKS - 1; i++)
			powerMelterSprite.push_back((CSprites::GetInstance()->Get(SPRITE_FILLARROW_ID)));
	}
}

void HUD::Render() {
	CSprites::GetInstance()->Get(SPRITE_HUD_ID)->Draw(x, y);

	if (typeHud == 0) {
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
	// for coin
	for (unsigned int i = 0; i < moneySprites.size(); i++) {
		moneySprites[i]->Draw(x + FONT_BBOX_WIDTH * i + HUD_DIFF_MONEY, y - HUD_DIFF_ROW);
	}

	// for time
	for (unsigned int i = 0; i < remainTimeSprites.size(); i++) {
		remainTimeSprites[i]->Draw(x + FONT_BBOX_WIDTH * i + HUD_DIFF_TIME, y + HUD_DIFF_ROW);
	}

	// for mario life
	for (unsigned int i = 0; i < mariolifeSprites.size(); i++)
		mariolifeSprites[i]->Draw(x + FONT_BBOX_WIDTH * i - HUD_DIFF_LIFE , y + HUD_DIFF_ROW);

	// for mario sprite
	playerSprite->Draw(x - HUD_DIFF_PLAYER, y + 4);

	// for score
	for (unsigned int i = 0; i < scoreSprites.size(); i++) {
		scoreSprites[i]->Draw(x + FONT_BBOX_WIDTH * i - HUD_DIFF_SCORE - 7, y + HUD_DIFF_ROW);
	}
}

void HUD::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	if (typeHud == 0) {
		AddSpeedStack();
		AddCoin();
		AddLife();
		AddScore();
	}
	// for mario life
	mariolifeSprites = StringToSprite(to_string(marioLife));

	// for coin
	moneySprites = StringToSprite(to_string(money));

	// for time
	time += dt;
	remainTime = DEFAULT_TIME - time / 1000;
	string time_str = to_string(remainTime);
	while (time_str.length() < HUD_TIME_MAX) time_str = "0" + time_str;
	remainTimeSprites = StringToSprite(time_str);

	// for score
	string score_str = to_string(score);
	while (score_str.length() < HUD_SCORE_MAX) score_str = "0" + score_str;
	scoreSprites = StringToSprite(score_str);
}

void HUD::AddSpeedStack() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	mario = currentScene->GetPlayer();
	this->speedStack = mario->speedStack;
}

void HUD::AddCoin() {
	if (mario != NULL)
		this->money = mario->coin;
}

void HUD::AddLife() {
	if (mario != NULL)
		this->marioLife = mario->marioLife;
}

void HUD::AddScore() {
	if (mario != NULL)
		this->score = mario->marioScore;
}

void HUD::LoadBackUpHud() {
	if (mario != NULL) {
		this->marioLife = mario->marioLife;
		this->score = mario->marioScore;
		this->money = mario->coin;
	}
}