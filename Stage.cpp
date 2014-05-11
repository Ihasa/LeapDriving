
#include "Stage.h"

LPDIRECT3DDEVICE9 CStage::Device=NULL;
// ���b�V��
vector<CMesh*> CStage::MeshList;
// �e�N�X�`��
vector<LPDIRECT3DTEXTURE9> CStage::TextureList;

//==========================================================
// Name :
// Desc : �R���X�g���N�^
//==========================================================
CStage::CStage()
{
}
//==========================================================
// Name :
// Desc : �f�X�g���N�^
//==========================================================
CStage::~CStage()
{
	for (size_t i=0; i<MeshList.size(); i++) MeshList[i]->Destroy();
	for (size_t i=0; i<TextureList.size(); i++) TextureList[i]->Release();
}

//==========================================================
// Name :
// Desc : �e�N�X�`���̃��[�h
//==========================================================
LPDIRECT3DTEXTURE9 CStage::LoadTexture(LPDIRECT3DDEVICE9 device, LPCSTR Filename)
{
	LPDIRECT3DTEXTURE9 texture;
	D3DXCreateTextureFromFileEx(device,Filename,0,0,0,0,D3DFMT_A8R8G8B8,
		D3DPOOL_MANAGED,D3DX_FILTER_LINEAR,D3DX_FILTER_LINEAR,0,NULL,NULL,&texture);
	TextureList.push_back(texture);
	return texture;
}

//==========================================================
// Name :
// Desc : ���b�V���̃��[�h
//==========================================================
CMesh* CStage::LoadMesh(LPDIRECT3DDEVICE9 device, LPCTSTR Filename) {
	CMesh* mesh=new CMesh();
	mesh->Create(device,Filename);
	MeshList.push_back(mesh);
	return mesh;
}

//==========================================================
// Name :
// Desc :
//==========================================================
//==========================================================
// Name :
// Desc :
//==========================================================
//==========================================================
// Name :
// Desc :
//==========================================================
//==========================================================
// Name :
// Desc :
//==========================================================