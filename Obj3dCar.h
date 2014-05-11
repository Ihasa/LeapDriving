//=====================================================================
// CObj3dCarクラスのインターフェース
//
//=====================================================================
#ifndef OBJ3DCAR_H
#define OBJ3DCAR_H


#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Obj3d.h"
#include "Mesh.h"


class CObj3dCar : public CObj3d
{
private:
/*	float		_acceleration;
	float		_velocity;
	float		_angularVelocity;
	float		_angularAcceleration;
*/
	//Tamama
	float		_acc;
	float		_vel;
	float		_angVel;
	float		_angAcc;
	float		dt;	//フレーム間時間
	float		maxVel;
	float		maxAng;
	int boostCounter;
	float curveVel;
	D3DXVECTOR3 _eye;
	D3DXVECTOR3 _at;
	

public:
	void accel();
	void accel(float maxV);
	void back();
	void brake(float val);
	void engineBrake(){brake(0.3f);}
	void turnRight(float val);
	void turnLeft(float val);
	void boost();
	void move();
	void camera();

/*
	void setAcceleration(float acceleration){_acceleration = acceleration;}
	void setVelocity(float velocity){_velocity = velocity;}
	float getVelocity() { return _velocity; }
	float getAcceleration() { return _acceleration; }
*/
	//Tamama
	void setAcceleration(float acc){_acc = acc;}
	void setVelocity(float vel){_vel = vel;}
	float getVelocity() { return _vel; }
	float getAcceleration() { return _acc; }
	void setAngVelZero() {_angVel = 0.0f;}		//Tamama追加　2009/8/25

	D3DXVECTOR3 getEye(){return _eye;}
	D3DXVECTOR3 getAt(){return _at;}
	float getVel(){return _vel;}

//	virtual void makeWorld();
//	virtual void draw();
	
	CObj3dCar();
	virtual ~CObj3dCar();
};

#endif