#include "BreakBrick.h"
#include "GameObject.h"
#include "Coin.h"
#include "PlayScene.h"
#include "PieceBrick.h"

void BreakableBrick::Render()
{
	if (isDeleted)
		return;
	animation_set->at(0)->Render(x, y);
}

void BreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {
	//
}

void BreakableBrick::Break() {
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(PIECE_ANI_SET_ID);

	BreakPiece* bPieceTopLeft = new BreakPiece(-1, 1);
	bPieceTopLeft->SetPosition(x, y);
	bPieceTopLeft->SetAnimationSet(ani_set);

	BreakPiece* bPieceBottomLeft = new BreakPiece(-1, -1);
	bPieceBottomLeft->SetPosition(x, y);
	bPieceBottomLeft->SetAnimationSet(ani_set);

	BreakPiece* bPieceTopRight = new BreakPiece(1, 1);
	bPieceTopRight->SetPosition(x, y);
	bPieceTopRight->SetAnimationSet(ani_set);

	BreakPiece* bPieceBottomRight = new BreakPiece(1, -1);
	bPieceBottomRight->SetPosition(x, y);
	bPieceBottomRight->SetAnimationSet(ani_set);

	// add to scene
	currentScene->AddObject(bPieceTopLeft);
	currentScene->AddObject(bPieceBottomLeft);
	currentScene->AddObject(bPieceTopRight);
	currentScene->AddObject(bPieceBottomRight);

	// delete BBrick
	isDeleted = true;
}