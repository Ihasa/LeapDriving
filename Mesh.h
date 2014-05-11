//=====================================================================
// CMesh: CMesh�N���X�̃C���^�[�t�F�[�X
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
	// ���b�V��
	LPD3DXMESH Mesh;

	// �}�e���A���̐��A�}�e���A��
	DWORD NumMaterials;
	D3DMATERIAL9* Materials;

	// �e�N�X�`��
	LPDIRECT3DTEXTURE9* Textures;

public:
	HRESULT Create(LPDIRECT3DDEVICE9 device, LPCSTR Filename);
	HRESULT Render(LPDIRECT3DDEVICE9 device);
	void Destroy();
	
	CMesh();
	~CMesh();
};

#endif