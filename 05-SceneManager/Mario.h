#pragma once
#include "GameObject.h"
#include "Animations.h"
#include "debug.h"
#include "Tail.h"
#include "Portal.h"

#define MARIO_WALKING_SPEED		0.1f
#define MARIO_RUNNING_SPEED		0.2f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0007f

#define MARIO_JUMP_SPEED_Y		0.25f
#define MARIO_JUMP_RUN_SPEED_Y	0.3f
#define MARIO_SLOW_FALLING_SPEED	0.03f

#define MARIO_ACCELERATION			0.07f

#define MARIO_GRAVITY_PIPE			0.00002f

#define MARIO_FLY_MAX 0.3f

#define MARIO_GRAVITY			0.0005f

#define MARIO_JUMP_DEFLECT_SPEED  0.2f

#define MARIO_BIG_DIF 7
#define MARIO_SIT_DIF 3

#define MARIO_RUNNING_STACKS		7
#define MARIO_ATTACK_STACKS			5

#pragma region STATE

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_STATE_TRANSFORM		11

#pragma endregion

#pragma region ANIMATION_ID

// SMALL
#define MARIO_ANI_SMALL_IDLE_RIGHT			0
#define MARIO_ANI_SMALL_WALKING_RIGHT		1
#define MARIO_ANI_SMALL_WALKING_FAST_RIGHT	2
#define MARIO_ANI_SMALL_RUNNING_RIGHT		3
#define MARIO_ANI_SMALL_JUMPINGUP_RIGHT		4
#define MARIO_ANI_SMALL_JUMPINGDOWN_RIGHT	5
#define MARIO_ANI_SMALL_BRAKING_RIGHT		6

#define MARIO_ANI_SMALL_IDLE_LEFT			7
#define MARIO_ANI_SMALL_WALKING_LEFT		8
#define MARIO_ANI_SMALL_WALKING_FAST_LEFT	9
#define MARIO_ANI_SMALL_RUNNING_LEFT		10
#define MARIO_ANI_SMALL_JUMPINGUP_LEFT		11
#define MARIO_ANI_SMALL_JUMPINGDOWN_LEFT	12
#define MARIO_ANI_SMALL_BRAKING_LEFT		13

//BIG
#define MARIO_ANI_BIG_IDLE_RIGHT			14
#define MARIO_ANI_BIG_WALKING_RIGHT			15
#define MARIO_ANI_BIG_WALKING_FAST_RIGHT	16
#define MARIO_ANI_BIG_RUNNING_RIGHT			17
#define MARIO_ANI_BIG_JUMPINGUP_RIGHT		18
#define MARIO_ANI_BIG_JUMPINGDOWN_RIGHT		19
#define MARIO_ANI_BIG_BRAKING_RIGHT			20
#define MARIO_ANI_BIG_SITTING_RIGHT			21

#define MARIO_ANI_BIG_IDLE_LEFT				22
#define MARIO_ANI_BIG_WALKING_LEFT			23
#define MARIO_ANI_BIG_WALKING_FAST_LEFT		24
#define MARIO_ANI_BIG_RUNNING_LEFT			25
#define MARIO_ANI_BIG_JUMPINGUP_LEFT		26
#define MARIO_ANI_BIG_JUMPINGDOWN_LEFT		27
#define MARIO_ANI_BIG_BRAKING_LEFT			28
#define MARIO_ANI_BIG_SITTING_LEFT			29

//TAIL
#define MARIO_ANI_TAIL_IDLE_RIGHT			30
#define MARIO_ANI_TAIL_WALKING_RIGHT		31
#define MARIO_ANI_TAIL_WALKING_FAST_RIGHT	32
#define MARIO_ANI_TAIL_RUNNING_RIGHT		33
#define MARIO_ANI_TAIL_JUMPINGUP_RIGHT		34
#define MARIO_ANI_TAIL_JUMPINGDOWN_RIGHT	35
#define MARIO_ANI_TAIL_BRAKING_RIGHT		36
#define MARIO_ANI_TAIL_SITTING_RIGHT		37

#define MARIO_ANI_TAIL_IDLE_LEFT			38
#define MARIO_ANI_TAIL_WALKING_LEFT			39
#define MARIO_ANI_TAIL_WALKING_FAST_LEFT	40
#define MARIO_ANI_TAIL_RUNNING_LEFT			41
#define MARIO_ANI_TAIL_JUMPINGUP_LEFT		42
#define MARIO_ANI_TAIL_JUMPINGDOWN_LEFT		43
#define MARIO_ANI_TAIL_BRAKING_LEFT			44
#define MARIO_ANI_TAIL_SITTING_LEFT			45

#define MARIO_ANI_DIE	62

//SMALL
#define MARIO_ANI_SMALL_HOLD_IDLE_RIGHT			63
#define MARIO_ANI_SMALL_HOLD_WALKING_RIGHT		64
#define MARIO_ANI_SMALL_HOLD_WALKING_FAST_RIGHT	64
#define MARIO_ANI_SMALL_HOLD_RUNNING_RIGHT		64
#define MARIO_ANI_SMALL_HOLD_JUMPINGUP_RIGHT	65
#define MARIO_ANI_SMALL_HOLD_JUMPINGDOWN_RIGHT	65
#define MARIO_ANI_SMALL_HOLD_BRAKING_RIGHT		64
#define MARIO_ANI_SMALL_KICKING_RIGHT			66

#define MARIO_ANI_SMALL_HOLD_IDLE_LEFT			67
#define MARIO_ANI_SMALL_HOLD_WALKING_LEFT		68
#define MARIO_ANI_SMALL_HOLD_WALKING_FAST_LEFT	68
#define MARIO_ANI_SMALL_HOLD_RUNNING_LEFT		68
#define MARIO_ANI_SMALL_HOLD_JUMPINGUP_LEFT		69
#define MARIO_ANI_SMALL_HOLD_JUMPINGDOWN_LEFT	69
#define MARIO_ANI_SMALL_HOLD_BRAKING_LEFT		68
#define MARIO_ANI_SMALL_KICKING_LEFT			70

//BIG
#define MARIO_ANI_BIG_HOLD_IDLE_RIGHT			71
#define MARIO_ANI_BIG_HOLD_WALKING_RIGHT		72
#define MARIO_ANI_BIG_HOLD_WALKING_FAST_RIGHT	72
#define MARIO_ANI_BIG_HOLD_RUNNING_RIGHT		72
#define MARIO_ANI_BIG_HOLD_JUMPINGUP_RIGHT		73
#define MARIO_ANI_BIG_HOLD_JUMPINGDOWN_RIGHT	73
#define MARIO_ANI_BIG_HOLD_BRAKING_RIGHT		72
#define MARIO_ANI_BIG_KICKING_RIGHT				74

#define MARIO_ANI_BIG_HOLD_IDLE_LEFT			75
#define MARIO_ANI_BIG_HOLD_WALKING_LEFT			76
#define MARIO_ANI_BIG_HOLD_WALKING_FAST_LEFT	76
#define MARIO_ANI_BIG_HOLD_RUNNING_LEFT			76
#define MARIO_ANI_BIG_HOLD_JUMPINGUP_LEFT		77
#define MARIO_ANI_BIG_HOLD_JUMPINGDOWN_LEFT		77
#define MARIO_ANI_BIG_HOLD_BRAKING_LEFT			76
#define MARIO_ANI_BIG_KICKING_LEFT				78

//TAIL
#define MARIO_ANI_TAIL_HOLD_IDLE_RIGHT			79
#define MARIO_ANI_TAIL_HOLD_WALKING_RIGHT		80
#define MARIO_ANI_TAIL_HOLD_WALKING_FAST_RIGHT	80
#define MARIO_ANI_TAIL_HOLD_RUNNING_RIGHT		80
#define MARIO_ANI_TAIL_HOLD_JUMPINGUP_RIGHT		81
#define MARIO_ANI_TAIL_HOLD_JUMPINGDOWN_RIGHT	81
#define MARIO_ANI_TAIL_HOLD_BRAKING_RIGHT		80
#define MARIO_ANI_TAIL_KICKING_RIGHT			82

#define MARIO_ANI_TAIL_HOLD_IDLE_LEFT			83
#define MARIO_ANI_TAIL_HOLD_WALKING_LEFT		84
#define MARIO_ANI_TAIL_HOLD_WALKING_FAST_LEFT	84
#define MARIO_ANI_TAIL_HOLD_RUNNING_LEFT		84
#define MARIO_ANI_TAIL_HOLD_JUMPINGUP_LEFT		85
#define MARIO_ANI_TAIL_HOLD_JUMPINGDOWN_LEFT	85
#define MARIO_ANI_TAIL_HOLD_BRAKING_LEFT		84
#define MARIO_ANI_TAIL_KICKING_LEFT				86

#define MARIO_ANI_TAIL_TURNING_RIGHT			95
#define MARIO_ANI_TAIL_TURNING_LEFT				96
#define MARIO_ANI_TAIL_FLAPPING_RIGHT			97
#define MARIO_ANI_TAIL_FLAPPING_LEFT			98

//FLY
#define MARIO_ANI_SMALL_FLY_RIGHT				103
#define MARIO_ANI_SMALL_FLY_LEFT				104
#define MARIO_ANI_BIG_FLY_RIGHT					105
#define MARIO_ANI_BIG_FLY_LEFT					106
#define MARIO_ANI_TAIL_FLY_UP_RIGHT				107
#define MARIO_ANI_TAIL_FLY_DOWN_RIGHT			108
#define MARIO_ANI_TAIL_FLY_FLAPPING_RIGHT		109
#define MARIO_ANI_TAIL_FLY_UP_LEFT				110
#define MARIO_ANI_TAIL_FLY_DOWN_LEFT			111
#define MARIO_ANI_TAIL_FLY_FLAPPING_LEFT		112
#pragma endregion

#define GROUND_Y 160.0f
// whack tail
#define MARIO_SPRITE_WHACK_LEFT_1_ID	12813
#define MARIO_SPRITE_WHACK_LEFT_2_ID	12814
#define MARIO_SPRITE_WHACK_LEFT_3_ID	12815
#define MARIO_SPRITE_WHACK_LEFT_4_ID	12816

#define MARIO_SPRITE_WHACK_RIGHT_1_ID	12803
#define MARIO_SPRITE_WHACK_RIGHT_2_ID	12804
#define MARIO_SPRITE_WHACK_RIGHT_3_ID	12805
#define MARIO_SPRITE_WHACK_RIGHT_4_ID	12806




#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL	3

#define MARIO_BIG_BBOX_WIDTH  16
#define MARIO_BIG_BBOX_HEIGHT 26
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 18

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT))

#define MARIO_SMALL_BBOX_WIDTH  12
#define MARIO_SMALL_BBOX_HEIGHT 14


#define MARIO_UNTOUCHABLE_TIME 2500
#define MARIO_FLYING_TIME		1750
#define MARIO_FLYING_TIME_MIN		200
#define MARIO_RUNNING_STACK_TIME	200
#define MARIO_ATTACK_TIME		350
#define MARIO_ATTACK_STACK_TIME 70


class CMario : public CGameObject
{
	
	float maxVx;
	
	
	
	ULONGLONG untouchable_start;
	
	

	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithQuestionBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithMushRoom(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopa(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaFire(LPCOLLISIONEVENT e);
	void OnCollisionWithFireBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithSwitch(LPCOLLISIONEVENT e);
	void OnCollisionWithPCardItem(LPCOLLISIONEVENT e);
	void OnCollisionWithHiddenBrick(LPCOLLISIONEVENT e);
	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdTail();
	

public:
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	int speedStack = 0;
	int attackStack = 0;
	int marioScore = 0;
	int marioLife = 0;
	int coin;
	int cardItemSc;
	BOOLEAN isFinish = false;
	BOOLEAN isSitting;
	CTail* tail = NULL;
	BOOLEAN isTailAttack = false;
	BOOLEAN isFlying = false;
	BOOLEAN isJumping = false;
	BOOLEAN isRunning = false;
	BOOLEAN isReadyToRun = false;
	BOOLEAN isOnPlatform;
	BOOLEAN isHolding = false;
	BOOLEAN isReadyToHold = false;
	BOOLEAN isTailFlying = false;
	BOOLEAN isTailFlyFlapping = false;
	BOOLEAN isFlapping = false;
	ULONGLONG tail_attack_start = 0;
	ULONGLONG attack_stack_start = 0;
	ULONGLONG tail_fly_start = 0;
	ULONGLONG tail_fly_min_start = 0;
	ULONGLONG fly_start = 0;
	ULONGLONG start_speed_stack = 0;
	ULONGLONG start_running = 0;
	int level;
	int untouchable;
	// switch map
	CPortal* portal = NULL;
	BOOLEAN isPipeUp = false;
	BOOLEAN isPipeDown = false;
	BOOLEAN isSwitchMap = false;
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 

		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 0;
		marioLife = 4;
		cardItemSc = 0;
		tail = new CTail(180, y);
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	void UpdateCoin() { this->coin++; }
	void AddScore(float x, float y, int score);
	int GetMarioDirection();
	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }

	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void HandleMarioJump();
	void HandleFlying();
	void HandleFlapping();
	void HandleSpeedStack();
	void HandleSwitchMap();
	void HandleFinishMap();
	void StartTailFlying() { tail_fly_start = GetTickCount64(); }
	void StartMinTailFlying() { tail_fly_min_start = GetTickCount64(); }
	void StartFlying() { fly_start = GetTickCount64(); }
	void StartPipeUp() {
		isPipeUp = true;

	}
	void StartPipeDown() {
		isPipeDown = true;
	}
	void StopPipeUp() {
		isPipeUp = false;
	}
	void StopPipeDown() {
		isPipeDown = false;
	}
	void StartAttack();
	void HandleAttack();
	void SetLevel(int l);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	void StartExtraMap() {
		SetPosition(180, 0);
	};

	void HackPosition() { x = 2265; y = 80; }
	
};