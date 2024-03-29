#include <iostream>
#include <fstream>

#include "WorldScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Brick.h"
#include "debug.h"
#include "WMObject.h"
#include "Mario.h"

#define WORLDOBJECT		10

using namespace std;

CWorldScene::CWorldScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	hud = NULL;
	key_handler = new CWorldSceneKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

void CWorldScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);

	CTextures::GetInstance()->Add(texID, path.c_str());

}

void CWorldScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CWorldScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CWorldScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id
	//DebugOut(L"--> %s\n", ToWSTR(line).c_str());
	int ani_set_id = atoi(tokens[0].c_str());
	LPANIMATION_SET s;
	if (CAnimationSets::GetInstance()->animation_sets[ani_set_id] != NULL)
		s = CAnimationSets::GetInstance()->animation_sets[ani_set_id];
	else
		s = new CAnimationSet();
	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}
	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CWorldScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y
	int tag = 0, option_tag_1 = 0, option_tag_2 = 0;
	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());
	if (tokens.size() >= 5)
		tag = atoi(tokens[4].c_str());
	if (tokens.size() >= 6)
		option_tag_1 = atoi(tokens[5].c_str());
	if (tokens.size() >= 7)
		option_tag_2 = atoi(tokens[6].c_str());


	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_PLAYER:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] PLAYER object was created before!\n");
			return;
		}
		obj = new CWorldPlayer(x, y);
		player = (CWorldPlayer*)obj;
		DebugOut(L"[INFO] Player object created! %f %f\n", x, y);
		break;
	case WORLDOBJECT:
		if (tag == OBJECT_TYPE_PORTAL || tag == OBJECT_TYPE_STOP)
		{
			bool cgLeft, cgRight, cgUp, cgDown;
			cgLeft = atof(tokens[5].c_str());
			cgUp = atof(tokens[6].c_str());
			cgRight = atof(tokens[7].c_str());
			cgDown = atof(tokens[8].c_str());
			int sceneid = atof(tokens[9].c_str());
			obj = new CWorldMapObject(sceneid);
			((CWorldMapObject*)obj)->SetMove(cgLeft, cgUp, cgRight, cgDown);
			obj->SetTag(tag);
		}
		else
		{
			obj = new CWorldMapObject();
			obj->SetTag(tag);
			if (tag == OBJECT_TYPE_HAMMER)
				obj->SetSpeed(MARIO_WALKING_SPEED_MIN / 2, 0);
		}
		break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	obj->SetAnimationSet(ani_set);

	objects.push_back(obj);
}
void CWorldScene::_ParseSection_TILEMAP_DATA(string line)
{
	int ID, rowMap, columnMap, columnTile, rowTile, totalTiles;
	LPCWSTR path = ToLPCWSTR(line);
	ifstream f;

	f.open(path);
	f >> ID >> rowMap >> columnMap >> rowTile >> columnTile >> totalTiles;
	//Init Map Matrix

	int** TileMapData = new int* [rowMap];
	for (int i = 0; i < rowMap; i++)
	{
		TileMapData[i] = new int[columnMap];
		for (int j = 0; j < columnMap; j++)
			f >> TileMapData[i][j];
	}
	f.close();

	current_map = new CMap(ID, rowMap, columnMap, rowTile, columnTile, totalTiles);
	current_map->ExtractTileFromTileSet();
	current_map->SetTileMapData(TileMapData);
}
void CWorldScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[TILEMAP DATA]") {
			section = SCENE_SECTION_TILEMAP_DATA; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }
		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_TILEMAP_DATA: _ParseSection_TILEMAP_DATA(line); break;
		}
	}

	f.close();

	hud = new HUD(WORLDSCENE_HUD);

	CGame::GetInstance()->SetCamPos(-5, 0);
	hud->SetPosition(130, current_map->GetMapHeight() + 10);

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CWorldScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	if (isUnLoaded)
		return;
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{

			coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
		objects[i]->Update(dt, &coObjects);

	hud->Update(dt, &coObjects);
}
void CWorldScene::Render()
{
	current_map->DrawMap();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
	hud->Render();
}

/*
	Unload current scene
*/
void CWorldScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	objects.clear();

	delete current_map;
	delete hud;
	player = nullptr;
	current_map = nullptr;
	hud = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}
void CWorldSceneKeyHandler::OnKeyDown(int KeyCode)
{
	CWorldPlayer* player = ((CWorldScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (player != NULL)
	{
		switch (KeyCode)
		{
		case DIK_RIGHT:
			if (player->cgRight)
				player->SetState(PLAYER_STATE_RIGHT);
			break;
		case DIK_LEFT:
			if (player->cgLeft)
				player->SetState(PLAYER_STATE_LEFT);
			break;
		case DIK_UP:
			if (player->cgUp)
				player->SetState(PLAYER_STATE_UP);
			break;
		case DIK_DOWN:
			if (player->cgDown)
				player->SetState(PLAYER_STATE_DOWN);
			break;
		case DIK_S:
		{
			if (player->readySwitchScene) {
				DebugOut(L"mario go to play scene \n");
				player->GoToPlayScene();
			}
		}

		break;
		}
	}
}