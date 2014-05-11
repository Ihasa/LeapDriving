#pragma once

#include "Leap.h"
using namespace Leap;
class MyGesture :
	public Gesture
{
public:
	MyGesture(void);
	~MyGesture(void);
	virtual Gesture::State state();
};
