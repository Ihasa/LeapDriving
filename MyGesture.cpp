#include "MyGesture.h"

MyGesture::MyGesture(void)
{
}

MyGesture::~MyGesture(void)
{
}

Gesture::State MyGesture::state()
{
	HandList hands = this->hands();
	if(hands.count() == 2 && hands.leftmost().fingers().count() != 5 && hands.rightmost().fingers().count() != 5)
	{
		return State::STATE_UPDATE;
	}
	else
	{
		return State::STATE_INVALID;
	}
}
