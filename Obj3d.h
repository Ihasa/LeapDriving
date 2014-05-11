//=====================================================================
// cObj3dクラスのインターフェース
//
//=====================================================================
#ifndef OBJ3D_H
#define OBJ3D_H


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Mesh.h"


class CObj3d
{
private:
	D3DXMATRIX _mScale;
	D3DXMATRIX _mRotate;
	D3DXMATRIX _mTrans;
	D3DXVECTOR3 _vScale;
	D3DXVECTOR3 _vRot;
	D3DXVECTOR3 _vPos;

public:
	D3DXVECTOR3 getPos(){return _vPos;}
	D3DXVECTOR3 getRot(){return _vRot;}
	
	void setScale(D3DXVECTOR3 vScale){_vScale = vScale;}
	void setRot(D3DXVECTOR3 vRot){_vRot = vRot;}
	void setPos(D3DXVECTOR3 vPos){_vPos = vPos;}
	void setPosX(float x){_vPos.x = x;}
	void setPosY(float y){_vPos.y = y;}
	void setPosZ(float z){_vPos.z = z;}
	


	
	virtual void makeWorld(D3DXMATRIX &world);
	void setWorld(LPDIRECT3DDEVICE9 device, D3DXMATRIX &world);
	virtual void draw(LPDIRECT3DDEVICE9 device, CMesh* mesh);

	CObj3d();
	virtual ~CObj3d();
};

//=====================================================================
// CViewクラスのインターフェース
//
//=====================================================================
class CView
{
private:
	D3DXVECTOR3 _from;
	D3DXVECTOR3 _lookat;
	D3DXVECTOR3 _up;

public:
	void setFrom(D3DXVECTOR3 from){_from = from;}
	void setFrom(float x, float y, float z){ _from.x = x, _from.y = y, _from.z = z;}
	void setLookat(D3DXVECTOR3 lookat){_lookat = lookat;}
	void setLookat(float x, float y, float z){ _lookat = D3DXVECTOR3(x,y,z);}
	void setUp(D3DXVECTOR3 up){_up = up;}
	D3DXVECTOR4 getFrom();

	void makeView(D3DXMATRIX &view);
	void setView(LPDIRECT3DDEVICE9 device, D3DXMATRIX &view);

	CView();
	~CView();
};

//=====================================================================
// CProjクラスのインターフェース
//
//=====================================================================
class CProj
{
private:
	float _fov;
	float _aspect;
	float _near;
	float _far;

public:
	void setFov(float fov){_fov = fov;}
	void setAspect(float aspect){_aspect = aspect;}
	void setRange(float zn, float zf){_near = zn; _far = zf;}
	

	void makeProj(D3DXMATRIX &proj);
	void setProj(LPDIRECT3DDEVICE9 device, D3DXMATRIX &proj);

	CProj();
	~CProj();
};

//=====================================================================
// CLightクラスのインターフェース
//
//=====================================================================
class CLight
{
private:
	D3DLIGHT9 light;

public:
	void Directional(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 direction);

	CLight();
	~CLight();
};
#endif
