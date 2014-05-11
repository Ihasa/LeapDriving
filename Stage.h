//=====================================================================
// CStage.h: CStageクラスのインターフェース
//
//=====================================================================
#ifndef STAGE_H
#define STAGE_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <DxErr.h>
#include <tchar.h>

#include "Mesh.h"
#include "Sprite.h"
#include <string>
using namespace std;
#include <vector>

class CStage
{
protected:
	static LPDIRECT3DDEVICE9 Device;
	
	static vector<CMesh*> MeshList;
	static vector<LPDIRECT3DTEXTURE9> TextureList;
	

public:
	static void SetDevice(LPDIRECT3DDEVICE9 device){Device = device;}
	static void DestroyMeshList(){for (size_t i=0; i<MeshList.size(); i++) MeshList[i]->Destroy();}
	static void ReleaseTextureList(){for (size_t i=0; i<TextureList.size(); i++) TextureList[i]->Release();}

	virtual void Draw(){};
	virtual void Move(){};

	CMesh* LoadMesh(LPDIRECT3DDEVICE9 device, LPCTSTR Filename);
	LPDIRECT3DTEXTURE9 CStage::LoadTexture(LPDIRECT3DDEVICE9 device, LPCTSTR Filename);
	
	CStage();
	virtual ~CStage();
};

#endif