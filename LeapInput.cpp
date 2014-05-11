#include "LeapInput.h"
#include <typeinfo.h>

LeapInput::LeapInput(void)
{
	leftHandY = rightHandY = 0;
	hands = 0;
	leftFingers = rightFingers = 0;
	accelGesture = brakeGesture = false;
	boost = false;
	back = false;
	active = false;
	curve = 0;
}

LeapInput::~LeapInput(void)
{
}

void LeapInput::onConnect(const Controller& controller)
{
	active = true;
}
void LeapInput::onFrame(const Controller& controller)
{
	Frame frame = controller.frame();
	HandList hands = frame.hands();
	this->hands = hands.count();
	if(hands.count() == 2)
	{
		Hand leftHand = hands.leftmost();
		Hand rightHand = hands.rightmost();
		leftHandY = leftHand.palmPosition().y;
		rightHandY = rightHand.palmPosition().y;
		leftFingers = leftHand.fingers().count();
		rightFingers = rightHand.fingers().count();

		float threshold = tanf(5* 3.141592f / 180.0f);
		float maxTan = tanf(45 * 3.141592f / 180.0f);
		if(leftHand.palmPosition().x != rightHand.palmPosition().x){
			float tanValue = fabs((leftHand.palmPosition().y - rightHand.palmPosition().y) / (leftHand.palmPosition().x - rightHand.palmPosition().x));
			if(tanValue > threshold){
				curve = tanValue  / maxTan;
				if(curve > 1)
					curve = 1;
			}else
				curve = 0;
		}else
			curve = 0;
	}
	else
	{
		leftHandY = rightHandY = 0;
	}

	boost = false;
	GestureList gestureList = frame.gestures();
	for(GestureList::const_iterator i = gestureList.begin(); i != gestureList.end(); ++i)
	{
		Gesture gesture = *i;
		if(gesture.state() != Gesture::State::STATE_INVALID)
		{
			switch(gesture.type())
			{
			case Gesture::Type::TYPE_SCREEN_TAP:
				boost = true;
				break;
			}
			
		}
	}

	back = false;
	if(hands.count() == 2 && leftFingers == 1 && rightFingers == 1 &&
		hands.leftmost().fingers()[0].direction().z > 0 &&
		hands.rightmost().fingers()[0].direction().z > 0)
	{
		back = true;
	}

	//accelGesture = brakeGesture = false;
	//GestureList gestureList = frame.gestures();
	//for(GestureList::const_iterator i = gestureList.begin(); i != gestureList.end(); ++i)
	//{
	//	Gesture gesture = *i;
	//	if(gesture.state() != Gesture::State::STATE_INVALID)
	//	{
	//		if(typeid(gesture) == typeid(MyGesture))
	//		{
	//			accelGesture = true;
	//		}
	//		
	//	}
	//}
}
float LeapInput::getLeftHandY()
{
	return leftHandY;
}

float LeapInput::getRightHandY()
{
	return rightHandY;
}

int LeapInput::HandCount()
{
	return hands;
}
bool LeapInput::gestureAccel()
{
	if(hands >= 2 && leftFingers != 5 && rightFingers != 5){
		return true;
	}
	return false;
	//return accelGesture;
}
bool LeapInput::gestureBrake()
{
	if(leftFingers != 0 && rightFingers != 0)
		return true;
	return false;
	//return brakeGesture;
}