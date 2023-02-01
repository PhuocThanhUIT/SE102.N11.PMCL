#pragma once
#include "GameObject.h"

#define OBJECT_TYPE_PLAYER				0
#define OBJECT_TYPE_BRICK				1
#define OBJECT_TYPE_STOP				2
#define OBJECT_TYPE_PORTAL				3
#define OBJECT_TYPE_BUSH				4
#define OBJECT_TYPE_HAMMER				5

#define HAMMER_LIMIT_RIGHT		84
#define HAMMET_LIMIT_LEFT       46
#define OBJECT_BBOX_WIDTH	4

#define START_WALK_LIMIT	10000
class CWorldMapObject :
	public CGameObject
{
	int sceneId;

public:
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual int IsBlocking() { return 0; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	CWorldMapObject(int sceneId = -1);
	int GetSceneId() { return sceneId; };
	bool cgLeft, cgRight, cgUp, cgDown;
	void SetMove(bool cLeft, bool cUp, bool cRight, bool cDown)
	{
		cgLeft = cLeft;
		cgRight = cRight;
		cgUp = cUp;
		cgDown = cDown;
	};
	void GetMove(bool& cLeft, bool& cUp, bool& cRight, bool& cDown)
	{
		cLeft = cgLeft;
		cRight = cgRight;
		cUp = cgUp;
		cDown = cgDown;
	};
};
