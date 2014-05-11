
#include "Obj3dCar.h"

//==========================================================
// Name :
// Desc : コンストラクタ
//==========================================================
CObj3dCar::CObj3dCar() : CObj3d()
{
	//Tamama
	_vel = 0.0f;
	_acc = 0.2f;			// dt*dtで割った値

	_angVel = 0.0f;
	_angAcc = 4.5f;			// dt*dtで割った値

	dt = 0.016666667f;			// フレーム間時間　1/60[s]

	boostCounter = 0;

	curveVel = 0.12f;
	maxVel = 0.6f;	
	maxAng = 1.5f;
	this->setPos(D3DXVECTOR3(0.0f,0.2f,0.0f));
}

//==========================================================
// Name :
// Desc : デストラクタ
//==========================================================
CObj3dCar::~CObj3dCar()
{
}

//==========================================================
// Name : accel()
// Desc : アクセル
//==========================================================
void CObj3dCar::accel()
{
	_vel += _acc * dt;
	if(_vel > maxVel) 
		_vel = maxVel;
}
void CObj3dCar::accel(float maxV)
{
	_vel += _acc * dt;
	if(_vel > maxV) 
		_vel = maxV;
}

//==========================================================
// Name :
// Desc : ハンドル（左）
//==========================================================
void CObj3dCar::turnLeft(float val)
{
	if(fabs(_vel) > curveVel){
		if(_vel > 0)
			_angVel -= _angAcc * dt * val;
		else
			_angVel += _angAcc * dt * val;
		if(_angVel < -maxAng*val) 
			_angVel = -maxAng*val;
	}

}

//==========================================================
// Name :
// Desc : ハンドル（右）
//==========================================================
void CObj3dCar::turnRight(float val)
{
	if(fabs(_vel) > curveVel){
		if(_vel > 0)
			_angVel += _angAcc * dt;
		else
			_angVel -= _angAcc * dt;
		if(_angVel > maxAng*val) 
			_angVel = maxAng*val;
	}

}
void CObj3dCar::boost()
{
	boostCounter = 60;
}
//==========================================================
// Name :
// Desc : バック
//==========================================================
void CObj3dCar::back()
{
	_vel -= _acc * dt;
	if( _vel < -maxVel/2)
		_vel = -maxVel/2;
}

//==========================================================
// Name :
// Desc : ブレーキ
//==========================================================
void CObj3dCar::brake(float val)
{
	_vel -= _acc*3*val * dt;
	if(_vel < 0)
		_vel = 0;

}

//==========================================================
// Name :
// Desc : 車の動き
//==========================================================
void CObj3dCar::move()
{

	D3DXVECTOR3	ang = getRot();
	D3DXVECTOR3 pos = getPos();

	ang.y += _angVel * dt;

	if(boostCounter != 0)
	{
		boostCounter--;
		_vel = maxVel*2;
	}

	pos.x += sinf(ang.y) * _vel;
	pos.z += cosf(ang.y) * _vel;
	//pos.z += 
	
	setRot(ang);
	setPos(pos);
	//camera();

}


//==========================================================
// Name :
// Desc : カメラ
//==========================================================
void CObj3dCar::camera()
{
	D3DXVECTOR3 rot = getRot();
	D3DXVECTOR3 pos = getPos();

	//Tamama
	_eye.x=pos.x-sinf(rot.y)*10;
	_eye.y=3.0f;
	_eye.z=pos.z-cosf(rot.y)*10;

	_at.x=pos.x;
	_at.y=1.0f;
	_at.z=pos.z;
}























