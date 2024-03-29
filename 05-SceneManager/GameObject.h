#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>
#include "Animations.h"
#include "Sprites.h"
#include "Collision.h"

using namespace std;

#define ID_TEX_BBOX 120		// special texture to draw object bounding box
#define BBOX_ALPHA 0.25f		// Bounding box transparency

class CGameObject
{
protected:

	

	int state;

	LPANIMATION_SET animation_set;

public:
	BOOLEAN isActive = false;
	bool isDeleted;
	int tag = 0; // define for object tag
	float start_x;
	float start_y;
	float x;
	float y;
	int nx;
	float vx;
	float vy;
	int isBlocking = 1;
	int typeHud;
	void SetTag(int tag) { this->tag = tag; }
	void SetActive(bool active) { this->isActive = active; }
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }
	int GetState() { return this->state; }
	int GetTag() { return this->tag; }
	virtual void Delete() { isDeleted = true;  }
	bool IsDeleted() { return isDeleted; }

	void RenderBoundingBox();

	CGameObject();
	CGameObject(float x, float y) :CGameObject() { this->x = x; this->y = y; }


	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL) {};
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; }

	//
	// Collision ON or OFF ? This can change depending on object's state. For example: die
	//
	virtual int IsCollidable() { return 0; };

	// When no collision has been detected (triggered by CCollision::Process)
	virtual void OnNoCollision(DWORD dt) {};

	// When collision with an object has been detected (triggered by CCollision::Process)
	virtual void OnCollisionWith(LPCOLLISIONEVENT e) {};
	
	// Is this object blocking other object? If YES, collision framework will automatically push the other object
	virtual int IsBlocking() { return 1; }

	virtual int SetIsBlocking(int block) { return this->isBlocking = block; }

	bool isColliding(float friend_left, float friend_top, float friend_right, float friend_bottom) {
		float this_left, this_top, this_right, this_bottom;

		GetBoundingBox(
			this_left,
			this_top,
			this_right,
			this_bottom);

		bool on1 = friend_left <= this_right;
		bool on2 = friend_top <= this_bottom;
		bool down1 = friend_right >= this_left;
		bool down2 = friend_bottom >= this_top;

		return on1 && on2 && down1 && down2;
	};

	void SetVx(float x) { this->vx = x; }

	float getX() { return x; }
	float getY() { return y; }

	~CGameObject();

	static bool IsDeleted(const LPGAMEOBJECT &o) { return o->isDeleted; }
};
