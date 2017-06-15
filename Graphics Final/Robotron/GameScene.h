//
//	Bachelor of Software Engineering
//	Media Design School
//	Auckland
//	New Zealand
//
//	(c) 2005 - 2016 Media Design School
//
//	File Name	:	GameScene.h
//	Description	:	GameScene declaration file
//	Author		:	Casey Chim
//	Mail		:	casey.chi6605@mediadesign.school.nz
//

#pragma once

#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include <iostream>
#include <vector>

#include "3DModel.h"
#include "GameEntity.h"
#include "Planet.h"
#include "Terrain.h"
//#include "Utils.h"

#include "Model.h"
#include "Camera.h"
#include "TextLabel.h"
#include "Cubemap.h"
#include "ReflectionModel.h"
#include "Light.h"
#include "GeometryModel.h"
#include "TessModel.h"
#include "HeightGenerator.h"
#include "ToonShadedModel.h"

enum Sounds
{
	MUSIC,
	CLICK,
	SHOOT,
	DEATH,
	LOSE,
	POWERUP,
	SIREN
};

class CPlayerShip;
class CEnemy;
class CLight;
class ToonShadedModel;

using namespace glm;

class GameScene
{
public:
	GameScene();
	~GameScene();
	void Init(HWND);
	void render();
	void update(unsigned char keyState[255]);
	void mouseUpdate( int _iX, int _iY);
	void SetPressed(bool _NewState);
	CCamera* GetCamera();
	CLight* GetLight();
	static GameScene& GetInstance();

	void ClearScreen();

	
private:
	HWND name;
	GLsizei WIDTH;
	GLsizei HEIGHT;
	GLsizei HalfWidth;
	GLsizei HalfHeight;
	//Camera member variables
	CCamera* MainCamera;
	float CamYaw;
	float CamPitch;
	float CamSensitivity;
	float m_fLastMouseX;
	float m_fLastMouseY;

	CLight* MainLight;
	Cubemap* m_pSkybox;
	HeightGenerator* hGenerator;
	CTerrain* m_pTerrain;
	ReflectionModel* m_pSphere;
	Model* m_p3DCastle;
	Model* m_pSuit;
	GeometryModel* m_pGeoModel;
	TessModel* m_pTessModel;
	ToonShadedModel* toonModel;


	FMOD::Channel* channel1;
	FMOD::Channel* channel2;
	FMOD::Channel* channel3;
	FMOD::Channel* channel4;

	FMOD::System* audioMgr;
	FMOD::Sound* death;
	FMOD::Sound* powerup;
	FMOD::Sound* bgMusic;
	FMOD::Sound* menuClick;
	FMOD::Sound* shoot;
	FMOD::Sound* lose;
	FMOD::Sound* siren;

	CGameEntity* m_pBox;
	CGameEntity* m_pMirror;
	CGameEntity* m_pInvertedBox;

	CGameEntity* m_pGround;
	TextLabel* m_pLabel;

	std::vector<CModel*> Models;
	std::vector<CGameEntity*> Entities;
	std::vector<TextLabel*> Labels;
	static GameScene* Instance;

	int m_iLevel;
	int m_iScene;
	int m_iMenuItem;
	bool Pressed;
	GLfloat DeltaTime;
	GLfloat LastTime;
	GLfloat m_fPowerUpCooldown;

	//Options
	bool m_bMSAA; 
	bool m_bWireframe;
	bool m_bCull;
	bool m_bDepth;
};

#endif