//=====================================================================
// CMesh: CMeshクラスのインターフェース
//
//=====================================================================
#ifndef MESH_H
#define MESH_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>

class CMesh
{
public:
	// メッシュ
	LPD3DXMESH Mesh;

	// マテリアルの数、マテリアル
	DWORD NumMaterials;
	D3DMATERIAL9* Materials;

	// テクスチャ
	LPDIRECT3DTEXTURE9* Textures;

public:
	HRESULT Create(LPDIRECT3DDEVICE9 device, LPCSTR Filename);
	HRESULT Render(LPDIRECT3DDEVICE9 device);
	void Destroy();
	
	CMesh();
	~CMesh();
};

#endif