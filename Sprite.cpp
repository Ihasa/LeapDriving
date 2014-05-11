
#include "Sprite.h"

LPD3DXSPRITE CSprite::Sprite = NULL;

//==========================================================
// Name :
// Desc : コンストラクタ
//==========================================================
CSprite::CSprite()
{
	_position = D3DXVECTOR3(0.0f,0.0f,NULL);
	_color = 0xFFFFFFFF;

}

//==========================================================
// Name :
// Desc : デストラクタ 
//==========================================================
CSprite::~CSprite()
{

}

//==========================================================
// Name : 
// Desc : 
//==========================================================
HRESULT CSprite::CreateSprite(LPDIRECT3DDEVICE9 device)
{
	D3DXCreateSprite(device, &Sprite);
	return S_OK;
}

//==========================================================
// Name :
// Desc : 描画
//==========================================================
void CSprite::Draw(LPDIRECT3DTEXTURE9 tex)
{
	Sprite->Begin(D3DXSPRITE_ALPHABLEND);
	Sprite->Draw(tex, NULL, NULL, &_position, _color);
	Sprite->End();
}



















