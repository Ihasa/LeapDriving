
#include "Mesh.h"

//==========================================================
// Name :
// Desc : コンストラクタ
//==========================================================
CMesh::CMesh()
{
	Mesh = NULL;
	NumMaterials = 0;
	Materials = NULL;
	Textures = NULL;
}

//==========================================================
// Name :
// Desc : デストラクタ
//==========================================================
CMesh::~CMesh()
{
	Destroy();
}

//==========================================================
// Name :
// Desc : 
//==========================================================
HRESULT CMesh::Create(LPDIRECT3DDEVICE9 device, LPCSTR Filename)
{

	// ファイルを読み込む
	LPD3DXBUFFER buf_materials=NULL;
	Mesh=NULL;
	if (FAILED(D3DXLoadMeshFromX(
			Filename, D3DXMESH_SYSTEMMEM, 
			device, NULL, &buf_materials, NULL, &NumMaterials,
			&Mesh))) return false;

	// マテリアルとテクスチャのための配列を確保する
	D3DXMATERIAL* materials=
		(D3DXMATERIAL*)buf_materials->GetBufferPointer();
	Materials=new D3DMATERIAL9[NumMaterials];
	Textures=new LPDIRECT3DTEXTURE9[NumMaterials];

	// マテリアルをコピーし，テクスチャを読み込む
	for (DWORD i=0; i<NumMaterials; i++ ) {

		// マテリアルをコピーする
		Materials[i]=materials[i].MatD3D;

		// アンビエント色を設定する
		Materials[i].Ambient=Materials[i].Diffuse;

		// テクスチャを読み込む
		Textures[i]=NULL;

		HRESULT hr = D3DXCreateTextureFromFile(device, (LPCSTR)materials[i].pTextureFilename, &Textures[i]);
		if(FAILED(hr)) Textures[i] = NULL;
	}

	// 最適化
	DWORD* adjacency=new DWORD[Mesh->GetNumFaces()*3];
	HRESULT result;
	result=Mesh->GenerateAdjacency(0, adjacency);
	result=Mesh->OptimizeInplace(D3DXMESHOPT_STRIPREORDER, adjacency, NULL, NULL, NULL);
	delete adjacency;

	// 後片づけ
	buf_materials->Release();


	return S_OK;
}

//==========================================================
// Name :
// Desc : 
//==========================================================
HRESULT CMesh::Render(LPDIRECT3DDEVICE9 device)
{
	for( DWORD i=0; i<NumMaterials; i++ )
	{
	//	if( Materials[i].Diffuse.a == 1.0f )
	//		continue;
		// Set the material and texture
		device->SetMaterial( &Materials[i] );
		device->SetTexture( 0, Textures[i] );
		Mesh->DrawSubset(i);
	}
	return S_OK;
}

//==========================================================
// Name :
// Desc : 
//==========================================================
void CMesh::Destroy()
{
	if (Textures) {
		for (DWORD i=0; i<NumMaterials; i++) {
			if (Textures[i]) Textures[i]->Release();
		}
		delete[] Textures;
	}
	if (Materials) delete[] Materials;
	if (Mesh) Mesh->Release();
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