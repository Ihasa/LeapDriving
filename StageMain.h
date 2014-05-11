//=====================================================================
// CStageMain.h: CStageMainクラスのインターフェース
//
//=====================================================================
#ifndef STAGEMAIN_H
#define STAGEMAIN_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Mesh.h"
#include "Stage.h"
#include "Obj3d.h"
#include "Obj3dCar.h"
#include "Graphics.h"
#include "Input.h"
#include "LeapInput.h"

#include <string>
using namespace std;
#include <vector>

class CStageMain : public CStage
{
private:
	enum Scene
	{
		SCENE_MENU,
		SCENE_READY,
		SCENE_PLAYING
	};
	Scene currentScene;
	void drawMenu();
	void drawReady();
	void drawPlaying();
	void updateMenu();
	void updateReady();
	void updatePlaying();

	void drawCource(D3DXMATRIX view,D3DXMATRIX projection,D3DXMATRIX world);
	void drawCar(D3DXMATRIX view,D3DXMATRIX projection,D3DXMATRIX world);
	void defaultLighting();

	int readyCount;
protected:
	CObj3dCar* car;
	CMesh*  mcar;
	CObj3d* skybox;
	CMesh*  mskybox;
	CObj3d* cource;
	CMesh*   mcource[11];
	CView*  view;
	CProj*  proj;
	CLight* light;
	CSprite* sprite;
	CInput* key;
	LPDIRECT3DTEXTURE9 tex;

	Controller controller;
	LeapInput listener;
	bool backing;

	void leapUpdate();

public:
	
	virtual void Draw();
	virtual void Move();
	void keyBoard();
	void courceInit();
	void courceCollision(CObj3dCar* car);
	int CollisionDetection(D3DXVECTOR2& carPosition, float radius, D3DXVECTOR2& start, D3DXVECTOR2& end);

	// ジョイスティック処理		Tamama 2013/8/20
	void JoyStick_Move(void);

	CStageMain();
	virtual ~CStageMain();
};

#endif