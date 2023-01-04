#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Map.h"
//#include "Koopas.h"


class CPlayScene: public CScene
{
protected: 
	// A play scene has to have player, right? 
	CMario* player;					
	CMap* current_map = NULL;
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseObjFromFile(LPCWSTR path);
	void _ParseSection_TILEMAP_DATA(string line);
	
	
public: 
	bool isTurnOnCamY = false;
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	CMario* GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();
	virtual void SetCam(float cx, float cy, DWORD dt = 0);
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	void AddObject(LPGAMEOBJECT obj) { this->objects.insert(this->objects.begin()+1, obj); }// add obj to scene
	void AddObjectEnd(LPGAMEOBJECT obj) { this->objects.push_back(obj); }
	vector<LPGAMEOBJECT> GetObjects() { return this->objects; }
};

typedef CPlayScene* LPPLAYSCENE;

