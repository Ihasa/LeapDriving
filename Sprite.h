//=====================================================================
// Sprite.h: cSpriteクラスのインターフェース
//
//=====================================================================
#ifndef SPRITE_H
#define SPRITE_H

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>


class CSprite
{
private:
	static LPD3DXSPRITE Sprite;
	D3DXVECTOR3 _center;
	D3DXVECTOR3 _position;
	const RECT	_rect;
	D3DCOLOR	_color;

public:
	D3DXVECTOR3 getPos(){return _position;}
	void setPos(D3DXVECTOR3 position){_position = position;}

	static HRESULT CreateSprite(LPDIRECT3DDEVICE9 device);
	static void ReleaseSprite(){Sprite->Release();}
	virtual void Draw(LPDIRECT3DTEXTURE9 tex);


	CSprite();
	~CSprite();
	
};

#endif