// Ŭnicode please 
#include "stdafx.h"
#ifdef USE_LEAP_MOTION
#include "leapmotion.h"

using namespace Leap;

void IrrLeapListener::onFrame(const Controller& controller)
{
	const Frame frame = controller.frame();

	evt.frameId = frame.id();
	evt.fingers = frame.fingers();
	evt.hands = frame.hands();
	evt.tools = frame.tools();
	evt.gestures = frame.gestures();
    evt.timestamp = frame.timestamp();  
}
#endif