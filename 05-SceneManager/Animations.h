#pragma once

#include <Windows.h>
#include <unordered_map>
#include "Sprites.h"

using namespace std;
/*
	Sprite animation
*/
class CAnimationFrame
{
	LPSPRITE sprite;
	DWORD time;

public:
	CAnimationFrame(LPSPRITE sprite, int time) { this->sprite = sprite; this->time = time; }
	DWORD GetTime() { return time; }
	LPSPRITE GetSprite() { return sprite; }
};

typedef CAnimationFrame* LPANIMATION_FRAME;

class CAnimation
{
	ULONGLONG lastFrameTime;
	int defaultTime;
	int currentFrame;
	vector<LPANIMATION_FRAME> frames;
public:
	CAnimation(int defaultTime = 100) { this->defaultTime = defaultTime; lastFrameTime = -1; currentFrame = -1; }
	void Add(int spriteId, DWORD time = 0);
	void Render(float x, float y);
};

typedef CAnimation* LPANIMATION;

class CAnimations
{
	static CAnimations* __instance;

	unordered_map<int, LPANIMATION> animations;

public:
	void Add(int id, LPANIMATION ani);
	LPANIMATION Get(int id);
	void Clear();

	static CAnimations* GetInstance();
};
typedef vector<LPANIMATION> CAnimationSet;

typedef CAnimationSet* LPANIMATION_SET;

/*
	Manage animation set database
*/
class CAnimationSets
{
	static CAnimationSets* __instance;


public:
	CAnimationSets();
	void Add(int id, LPANIMATION_SET ani);
	LPANIMATION_SET Get(unsigned int id);
	unordered_map<int, LPANIMATION_SET> animation_sets;
	void Clear()
	{
		for (auto x : animation_sets)
		{
			LPANIMATION_SET anis = x.second;
			delete anis;
		}
		animation_sets.clear();
	}
	static CAnimationSets* GetInstance();
};
