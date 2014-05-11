
#include "Obj3d.h"


//==========================================================
// Name :
// Desc : 
//==========================================================
CObj3d::CObj3d()
{
	_vScale = D3DXVECTOR3(1.0f,1.0f,1.0f);
	_vRot   = D3DXVECTOR3(0.0f,0.0f,0.0f);
	_vPos   = D3DXVECTOR3(0.0f,0.0f,0.0f);

}

//==========================================================
// Name :
// Desc : 
//==========================================================
CObj3d::~CObj3d()
{
}

//==========================================================
// Name :
// Desc : 
//==========================================================
void CObj3d::makeWorld(D3DXMATRIX &world)
{
	D3DXMatrixScaling(&_mScale,_vScale.x,_vScale.y,_vScale.z);
	D3DXMatrixRotationYawPitchRoll(&_mRotate,_vRot.y,_vRot.x,_vRot.z);
	D3DXMatrixTranslation(&_mTrans,_vPos.x,_vPos.y,_vPos.z);
	world = _mScale * _mRotate * _mTrans;
}

//==========================================================
// Name :
// Desc : 
//==========================================================
void CObj3d::setWorld(LPDIRECT3DDEVICE9 device, D3DXMATRIX &world)
{
	device->SetTransform(D3DTS_WORLD, &world);
}

//==========================================================
// Name :
// Desc : 
//==========================================================
void CObj3d::draw(LPDIRECT3DDEVICE9 device, CMesh* mesh)
{
	mesh->Render(device);
}

//=================================================================================================

//==========================================================
// Name :
// Desc : 
//==========================================================
CView::CView()
{
	_from = D3DXVECTOR3(1.0f,2.0f,-5.0f);
	_lookat = D3DXVECTOR3(0.0f,0.0f,0.0f);
	_up = D3DXVECTOR3(0.0f,1.0f,0.0f);
}

//==========================================================
// Name :
// Desc : 
//==========================================================
CView::~CView()
{
	
}

//==========================================================
// Name :
// Desc : 
//==========================================================
D3DXVECTOR4 CView::getFrom()
{
	D3DXVECTOR4 from;
	from.x = _from.x;
	from.y = _from.y;
	from.z = _from.z;
	from.w = 0;
	return from;
}

//==========================================================
// Name :
// Desc : 
//==========================================================
void CView::makeView(D3DXMATRIX &view)
{
	D3DXMatrixLookAtLH(&view,&_from,&_lookat,&_up);

}

//==========================================================
// Name :
// Desc : 
//==========================================================
void CView::setView(LPDIRECT3DDEVICE9 device, D3DXMATRIX &view)
{
	device->SetTransform(D3DTS_VIEW,  &view);

}

//==================================================================================================
//==========================================================
// Name :
// Desc : 
//==========================================================
CProj::CProj()
{
	_fov = 50.0f * D3DX_PI / 180.0f;
	_aspect = 1.0f;
	_near = 0.01f;
	_far = 500.0f;
}

//==========================================================
// Name :
// Desc : 
//==========================================================
CProj::~CProj()
{
}

//==========================================================
// Name :
// Desc : 
//==========================================================
void CProj::makeProj(D3DXMATRIX &proj)
{
	D3DXMatrixPerspectiveFovLH(&proj,_fov,_aspect,_near,_far);

}

//==========================================================
// Name :
// Desc : 
//==========================================================
void CProj::setProj(LPDIRECT3DDEVICE9 device, D3DXMATRIX &proj)
{
	device->SetTransform(D3DTS_PROJECTION, &proj);
}

//==================================================================================================
//==========================================================
// Name :
// Desc : 
//==========================================================
CLight::CLight()
{
}

//==========================================================
// Name :
// Desc : 
//==========================================================
CLight::~CLight()
{
}

//==========================================================
// Name :
// Desc : 
//==========================================================
void CLight::Directional(LPDIRECT3DDEVICE9 device, D3DXVECTOR3 direction)
{
	ZeroMemory(&light,sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
//	light.Type = D3DLIGHT_POINT;
	light.Ambient.r = 0.3f;				light.Ambient.g = 0.3f;				light.Ambient.b = 0.3f;
	light.Diffuse.r = 0.6f;				light.Diffuse.g = 0.6f;				light.Diffuse.b = 0.6f;
	light.Specular.r = 1.0f;			light.Specular.g = 1.0f;			light.Specular.b = 1.0f;
//	light.Position = D3DXVECTOR3(3.0f,3.0f,3.0f);
	light.Direction.x= direction.x;		light.Direction.y= direction.y;		light.Direction.z= direction.z;
//	light.Range = 30.0f;

	// ƒ‰ƒCƒg—LŒø‰»
	device->LightEnable(D3DLIGHT_DIRECTIONAL,TRUE);
	device->SetLight(D3DLIGHT_DIRECTIONAL,&light);
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