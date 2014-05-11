#pragma once

#include "Leap.h"
using namespace Leap;
class LeapInput : public Listener
{
private:
	float leftHandY,rightHandY;
	int hands;
	int leftFingers,rightFingers;
	bool accelGesture,brakeGesture;
	bool boost;
	bool back;
	bool active;

	float curve;
public:
	LeapInput(void);
	~LeapInput(void);
	float getLeftHandY();
	float getRightHandY();
	bool gestureAccel();
	bool gestureBrake();
	int HandCount();
	bool getBoost(){return boost;}
	bool getBack(){return back;}
	bool IsActive(){return active;}
	float GetCurve(){return curve;}
	virtual void onConnect(const Controller&);
	virtual void onFrame(const Controller&);
};
